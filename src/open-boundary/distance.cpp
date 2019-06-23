// Calculate the distance between particles.

#include <math.h>
#include "generator.h"
#include "boundary.h"

double distance(Point p, Point q) {
  double dx = p.x - q.x;
  double dy = p.y - q.y;
  return sqrt(dx * dx + dy * dy);
}

// The direction is always a->b.
double diff(double a, double b) {
  return b - a;
}

// Do nothing for open boundary.
double imaging(double x) {
  return x;
}
