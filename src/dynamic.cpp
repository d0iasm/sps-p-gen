// This is the part of main calculation. Basic calculation is almost same but the kparam changes dynamically.

#include <iostream>
#include <stdlib.h>
#include "energy.h"
#include "dynamic.h"

static double fRand(double min, double max) {
  return min + ((double) rand() / (double) RAND_MAX) * (max - min);
}

void updateKparam() {
  int i = rand() % NPOINTS;
  int j = rand() % NPOINTS;
  double oldk = kparam[i][j];
  double oldEnergy = energyAverage();
  //double oldEnergy = energyAverageDist();
  //kparam[i][j] = kparam[i][j] + fRand(-0.5, 0.5);
  kparam[i][j] = kparam[i][j] + ((rand() % 4 - 2) * 0.1);

  if (kparam[i][j] > 2.0 || kparam[i][j] < -2.0)
    kparam[i][j] = oldk;

  if (oldEnergy > energyAverage())
    kparam[i][j] = oldk;
}