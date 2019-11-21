#ifndef XV_H
#define XV_H

#include <math.h>
#include "generator.h"

struct XV {
  double x;
  double v;
};

// Global functions.
Point computeCenter();
XV computeXV(Point *dydx, Point p);

#endif // XV_H
