// This is the part of main calculation. Basic calculation is almost same but the kparam changes dynamically.

#include <iostream>
#include <stdlib.h>
#include "energy.h"
#include "dynamic.h"

static double fRand(double min, double max) {
  return min + ((double) rand() / (double) RAND_MAX) * (max - min);
}

// Update a K param based on the energy of a `whole` network.
static void updateKparamStatic() {
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
static void updateKparamDynamic() {
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
static void updateKparamLocalEnergy() {
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

// Update a K param based on K params in a local network. This doesn't use the energy.
static void updateKparamLocal() {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;

  double sum = 1.0;
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


// Dispatcher for how to update K params based on `dynamic` variable.
void updateKparam() {
  if (dynamic == "none") {
    return;
  }
  if (dynamic == "e-static") {
    updateKparamStatic();
    return;
  }
  if (dynamic == "e-dynamic") {
    updateKparamDynamic();
    return;
  }
  if (dynamic == "e-local") {
    updateKparamLocalEnergy();
    return;
  }
  if (dynamic == "local") {
    updateKparamLocal();
    return;
  }
}
