// This is the part of main calculation. Basic calculation is almost same but the kparam changes dynamically.

#include <iostream>
#include <stdlib.h>
#include "boundary.h"
#include "energy.h"
#include "dynamic.h"

static double fRand(double min, double max) {
  return min + ((double) rand() / (double) RAND_MAX) * (max - min);
}

// Update a K param based on the energy of a `whole` network.
// K param can take only 0.1 step values (discrete model).
static void updateGlobalStaticDiscrete() {
  int i = rand() % NPOINTS;
  int j = rand() % NPOINTS;
  double oldk = kparam[i][j];
  double oldEnergy = energyAverage();

  // Avoid a digit error.
  int tmp = (int) (kparam[i][j] * 10);
  int diff = 0;
  while (diff == 0) {
    // Get a number -1 or 1.
    diff = rand() % 3 - 1;
  }
  tmp += diff;
  kparam[i][j] = tmp / 10.0;

  if (kparam[i][j] > maxk || kparam[i][j] < mink)
    kparam[i][j] = oldk;

  if (oldEnergy < energyAverage())
    kparam[i][j] = oldk;
}

// Update a K param based on the energy of a `whole` network.
// K param can take only 0.1 step values (discrete model).
static void updateGlobalDynamicDiscrete() {
  int i = rand() % NPOINTS;
  int j = rand() % NPOINTS;
  double oldk = kparam[i][j];
  double oldEnergy = energyAverageDist();

  // Avoid a digit error.
  int tmp = (int) (kparam[i][j] * 10);
  int diff = 0;
  while (diff == 0) {
    // Get a number -1 or 1.
    diff = rand() % 3 - 1;
  }
  tmp += diff;
  kparam[i][j] = tmp / 10.0;

  if (kparam[i][j] > maxk || kparam[i][j] < mink)
    kparam[i][j] = oldk;

  if (oldEnergy < energyAverageDist())
    kparam[i][j] = oldk;
}

// Update a K param based on the energy of a `local` network.
// K param can take only 0.1 step values (discrete model).
static void updateLocalStaticDiscrete() {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;
  double oldk = kparam[p][o];
  double oldEnergy = energyLocal(p, o);

  // Avoid a digit error.
  int tmp = (int) (kparam[p][o] * 10);
  int diff = 0;
  while (diff == 0) {
    // Get a number -1 or 1.
    diff = rand() % 3 - 1;
  }
  tmp += diff;
  kparam[p][o] = tmp / 10.0;

  // Restore an old K param because new one exceeds min/max value.
  if (kparam[p][o] > maxk || kparam[p][o] < mink)
    kparam[p][o] = oldk;

  // Restore an old K param because the old energy is more stable (= low energy).
  if (oldEnergy < energyLocal(p, o)) {
    kparam[p][o] = oldk;
  }
}

// Update a K param based on the energy of a `local` network.
// K param can take only 0.1 step values (discrete model).
static void updateLocalDynamicDiscrete() {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;
  double oldk = kparam[p][o];
  double oldEnergy = energyLocalDist(p, o);

  // Avoid a digit error.
  int tmp = (int) (kparam[p][o] * 10);
  int diff = 0;
  while (diff == 0) {
    // Get a number -1 or 1.
    diff = rand() % 3 - 1;
  }
  tmp += diff;
  kparam[p][o] = tmp / 10.0;

  // Restore an old K param because new one exceeds min/max value.
  if (kparam[p][o] > maxk || kparam[p][o] < mink)
    kparam[p][o] = oldk;

  // Restore an old K param because the old energy is more stable (= low energy).
  if (oldEnergy < energyLocalDist(p, o)) {
    kparam[p][o] = oldk;
  }
}

// Update a K param based on K params in a local network. This doesn't use the energy.
// K param can take any values as long as ranging from MIN to MAX (continuous model).
static void updateLocalStaticContinuous() {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;

  double sum = 0.0;
  for (int x=0; x < NPOINTS; x++) {
    if (x == p || x == o)
      continue;

    sum += (kparam[p][x] * kparam[o][x]);
  }
  kparam[p][o] += (sum / (NPOINTS - 2));

  if (kparam[p][o] > maxk) {
    kparam[p][o] = maxk;
  }
  if (kparam[p][o] < mink) {
    kparam[p][o] = mink;
  }
}

// Update a K param based on K params influenced from the distance between paraticles
// in a local network. This doesn't use the energy.
// K param can take any values as long as ranging from MIN to MAX (continuous model).
static void updateLocalDynamicContinuous() {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;

  Point &pp = points[p];
  Point &po = points[o];

  double sum = 0.0;
  for (int x=0; x < NPOINTS; x++) {
    if (x == p || x == o)
      continue;

    Point &px = points[x];
    sum += (kparam[p][x] / distance(pp, px) * kparam[o][x] / distance(po, px));
  }
  kparam[p][o] += (sum / (NPOINTS - 2));

  if (kparam[p][o] > maxk) {
    kparam[p][o] = maxk;
  }
  if (kparam[p][o] < mink) {
    kparam[p][o] = mink;
  }
}

// Dispatcher for how to update K params based on `dynamic` variable.
void updateKparam() {
  if (dynamic == "none") {
    return;
  }
  if (dynamic == "global-static-discrete") {
    // global static energy
    updateGlobalStaticDiscrete();
    return;
  }
  if (dynamic == "global-dynamic-discrete") {
    // global dynamic energy
    updateGlobalDynamicDiscrete();
    return;
  }
  if (dynamic == "local-static-discrete") {
    // local static energy
    updateLocalStaticDiscrete();
    return;
  }
  if (dynamic == "local-dynamic-discrete") {
    // local static energy
    updateLocalDynamicDiscrete();
    return;
  }
  if (dynamic == "local-static-continuous") {
    // local Kparams
    updateLocalStaticContinuous();
    return;
  }
  if (dynamic == "local-dynamic-continuous") {
    // local dynamic Kparams
    updateLocalDynamicContinuous();
    return;
  }
}
