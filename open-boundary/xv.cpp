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

int classify() {
  XV s = xv[1];
  XV e = xv[xv.size() - 1]; 
  XV m = xv[xv.size() / 2];

  //std::cerr << "\n------------\n";
  //std::cerr << "start (x, v) " << s.x << ", " << s.v << "\n";
  //std::cerr << "mid   (x, v) " << m.x << ", " << m.v << "\n";
  //std::cerr << "end   (x, v) " << e.x << ", " << e.v << "\n";
  //std::cerr << "------------\n";

  if (s.x > m.x && m.x > e.x && s.v > m.v && m.v > e.v) {
    return 1; 
  } else if (s.v > m.v && m.v > e.v) {
    return 2;
  } else if (s.x > m.x && m.x > e.x) {
    return 3;
  } else if (e.x > 0.01 && e.v > 0.01) {
    return 4;
  }

  return 7; // class 5 or 6.
}
