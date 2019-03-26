#ifndef XV_H
#define XV_H

#include <math.h>
#include "generator.h"

struct XV {
  double x;
  double v;
};

// Graph
extern Point center;
extern std::vector<XV> xv;

Point computeCenter();
void computeXV(Point *delta);

#endif // XV_H
