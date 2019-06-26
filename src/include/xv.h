#ifndef XV_H
#define XV_H

#include <math.h>
#include "generator.h"

struct XV {
  double x;
  double v;
};

// Global variables for drawing a graph.
extern Point center;
extern std::vector<XV> xv;

// Global functions.
Point computeCenter();
XV computeXV(Point *dydx);
int classify();

#endif // XV_H
