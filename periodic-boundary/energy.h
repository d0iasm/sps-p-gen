#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "generator.h"

// Global variables.
extern std::vector<double> energy;

// Local functions.
static double heider(int pi, int pj, int pk);
static double heiderDist(int pi, int pj, int pk);

// Global functions.
Color getColor(int i);
double energyAve();
double energyVar();
double energyVarDist();

#endif // ENERGY_H
