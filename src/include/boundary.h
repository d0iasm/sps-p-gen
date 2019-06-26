#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "generator.h"

void initPoints();
double distance(Point p, Point q);
double diff(double a, double b);
double imaging(double x);
void importScript();
void printCycle();

#endif // BOUNDARY_H
