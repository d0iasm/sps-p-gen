#ifndef BOUNDARY_H
#define BOUNDARY_H

#include <string>
#include "generator.h"

void initPoints();
double distance(Point p, Point q);
double diff(double a, double b);
double imaging(double x);
void importScript();
void printCycle();
std::string boundary();

#endif // BOUNDARY_H
