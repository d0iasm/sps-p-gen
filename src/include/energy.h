#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "generator.h"

// Global functions.
double heider(int, int, int);
double heiderSpaceLocality(int, int, int);
double energyLow();
double energyHigh();
double energyAverage(int, int);
double energyAverageDist(int, int);
double energyVariance();
double energyVarianceDist();
double energyLocal(int p, int o);
double energyLocalDist(int p, int o);

double cost(int i);

#endif // ENERGY_H
