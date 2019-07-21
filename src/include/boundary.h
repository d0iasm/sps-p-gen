#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <string>
#include "generator.h"

double distance(Point p, Point q);
double diff(double a, double b);
double imaging(double x);
void importScript();
void printCycle();
double density();
std::string boundary();

// Global variable.
extern int cycle;

#endif // BOUNDARY_H
