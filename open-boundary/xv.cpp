// This file implements the macroscopic variables to understand the pattern
// of this model. X converges to zero when at least one of the particles
// moves an infinite distance from the center of gravity. V converges to
// zero when the relative velocities of all particles with respect to the
// center of gravity converge to zero.

#include <iostream>
#include "xv.h"

Point computeCenter() {
  double x = 0;
  double y = 0;
  for (Point &p : points) {
    x += p.x;
    y += p.y;
  }
  return {x / NPOINTS, y / NPOINTS};
}

XV computeXV(Point *dxdy) {
  // Compute X.
  double sum = 0;
  for (Point &p : points) {
    sum += distance(center, p);
  }
  double x = NPOINTS / sum;

  // Compute V.
  Point newCenter = computeCenter();
  double cx = newCenter.x - center.x;
  double cy = newCenter.y - center.y;
  Point c = {cx, cy};
  sum = 0;
  for (int i = 0; i < NPOINTS; i++) {
    Point d = dxdy[i];
    sum += distance(c, d);
  }
  double v = sum / NPOINTS;

  center = newCenter;
  return {x, v};
}

