#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "generator.h"

// Global variables.
extern std::vector<double> energy;

Color getColor(int i);
static double heider(int pi, int pj, int pk);
static double heider_dist(int pi, int pj, int pk);
double energy_ave();
double energy_var();
double energy_var_dist();

#endif // ENERGY_H