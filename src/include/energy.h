#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "generator.h"

// Global variables.
extern std::vector<std::vector<double> > energy;

// Local functions.
static double heider(int pi, int pj, int pk);
static double heiderDist(int pi, int pj, int pk);

// Global functions.
double energyAverage();
double energyAverageDist();
double energyVariance();
double energyVarianceDist();

double energyLocal(int p, int o);
double energyLocalDist(int p, int o);

#endif // ENERGY_H
