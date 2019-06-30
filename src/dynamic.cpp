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

  // Avoid a digit error.
  int tmp = (int) (kparam[i][j] * 10);
  tmp += rand() % 4;
  kparam[i][j] = tmp / 10.0;

  if (kparam[i][j] > maxk || kparam[i][j] < mink)
    kparam[i][j] = oldk;

  if (oldEnergy > energyAverage())
    kparam[i][j] = oldk;
}
