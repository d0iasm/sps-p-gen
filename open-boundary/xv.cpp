// This file implements the macroscopic variables to understand the pattern
// of this model. X converges to zero when at least one of the particles
// moves an infinite distance from the center of gravity. V converges to
// zero when the relative velocities of all particles with respect to the
// center of gravity converge to zero.

#include <cfloat>
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

  // Calculate center.
  double sumx = 0;
  double sumv = 0;
  for(auto const& a: xv) {
    sumx += a.x;
    sumv += a.v; 
  }
  double gx = sumx / xv.size();
  double gv = sumv / xv.size();

  // Calculate variance.
  sumx = 0;
  sumv = 0;
  for (auto const& a:xv) {
    sumx += (a.x - gx) * (a.x - gx);
    sumv += (a.v - gv) * (a.v - gv);
  }
  double varx = sumx / xv.size();
  double varv = sumv / xv.size();

  std::cerr << "\n------------\n";
  std::cerr << "start (x, v) " << s.x << ", " << s.v << "\n";
  std::cerr << "mid   (x, v) " << m.x << ", " << m.v << "\n";
  std::cerr << "end   (x, v) " << e.x << ", " << e.v << "\n";
  std::cerr << "\ngx, gv: " << gx << ", " << gv << "\n"; 
  std::cerr << "\nvx, vv: " << varx << ", " << varv << "\n"; 
  std::cerr << "------------\n";

  if (e.x <= DBL_EPSILON && e.v <= DBL_EPSILON) {
    return 1; 
  } else if (e.v <= DBL_EPSILON) {
    return 2;
  } else if (e.x <= DBL_EPSILON) {
    return 3;
  } else if (s.x > m.x && m.x > e.x && s.v > m.v && m.v > e.v) {
    return 4;
  } else if (varx < FLT_EPSILON && varv < FLT_EPSILON) {
    return 5;
  } else if (varx > 1 && varv > 1) {
    return 6; 
  }

  return 0; 
}
