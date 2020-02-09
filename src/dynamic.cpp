// This is the part of main calculation. Basic calculation is almost same but the kparam changes dynamically.

#include <iostream>
#include <stdlib.h>
#include "boundary.h"
#include "energy.h"
#include "dynamic.h"

typedef double EnergyFn(int, int);

static void updateDiscrete(EnergyFn *energy) {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;
  // TODO: AVOID self preference!!!
  //while (p == o) {
    //o = rand() % NPOINTS;
  //}

  double oldk = kparam[p][o];

  double oldEnergy = energy(p, o);

  // Avoid a digit error.
  int tmp = (int) (kparam[p][o] * 10);
  int diff = 0;
  while (diff == 0) {
    // Get a number -1 or 1.
    diff = rand() % 3 - 1;
  }
  tmp += diff;
  kparam[p][o] = tmp / 10.0;

  // Align kapram with a min/max value.
  if (kparam[p][o] > maxk) {
    kparam[p][o] = maxk;
  }
  if (kparam[p][o] < mink) {
    kparam[p][o] = mink;
  }

  // Restore an old K param because the old energy is more stable (= low energy).
  if (oldEnergy < energy(p, o))
    kparam[p][o] = oldk;
}

// Update a K param based on K params in a local network. This doesn't use the energy.
// K param can take any values as long as ranging from MIN to MAX (continuous model).
static void updateContinuous(EnergyFn *energy) {
  int p = rand() % NPOINTS;
  int o = rand() % NPOINTS;
  double oldk = kparam[p][o];

  double oldEnergy = energy(p, o);

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

  // Restore an old K param because the old energy is more stable (= low energy).
  if (oldEnergy < energy(p, o)) {
    kparam[p][o] = oldk;
  }
}

// Dispatcher for how to update K params based on `dynamic` variable.
void updateKparam() {
  if (dynamic == "none") {
    return;
  }
  if (dynamic == "global-static-discrete") {
    // global static energy
    updateDiscrete(energyAverage);
    return;
  }
  if (dynamic == "global-dynamic-discrete") {
    // global dynamic energy
    updateDiscrete(energyAverageDist);
    return;
  }
  if (dynamic == "local-static-discrete") {
    // local static energy
    updateDiscrete(energyLocal);
    return;
  }
  if (dynamic == "local-dynamic-discrete") {
    // local static energy
    updateDiscrete(energyLocalDist);
    return;
  }
  if (dynamic == "local-static-continuous") {
    // local Kparams
    updateContinuous(energyLocal);
    return;
  }
  if (dynamic == "local-dynamic-continuous") {
    // local dynamic Kparams
    updateContinuous(energyLocalDist);
    return;
  }
}
