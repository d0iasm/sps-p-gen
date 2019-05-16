#ifndef ENERGY_H
#define ENERGY_H

#include <vector>
#include "generator.h"

// Global variables.
extern std::vector<std::vector<double> > energy;

// Local functions.
static double heider(int pi, int pj, int pk);
static double heider_dist(int pi, int pj, int pk);

// Global functions.
Color get_color(int i);
double energy_ave();
double energy_ave_dist();
double energy_var();
double energy_var_dist();

#endif // ENERGY_H
