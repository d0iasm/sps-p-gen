#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <string>
#include "generator.h"

double distance(Point p, Point q);
double diff(double a, double b);
double imaging(double x);

std::string getBoundary();
int getCycle();
double getDensity();
std::string getScript();

// Global variable.
extern int cycle;

#endif // BOUNDARY_H
