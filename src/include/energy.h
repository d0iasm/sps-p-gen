#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "generator.h"

// Global variables.
extern std::vector<std::vector<double> > energy;

// Global functions.
double energyMax();
double energyMin();
double energyAverage(int, int);
double energyAverageDist(int, int);
double energyVariance();
double energyVarianceDist();
double energyLocal(int p, int o);
double energyLocalDist(int p, int o);

double cost(int i);

#endif // ENERGY_H
