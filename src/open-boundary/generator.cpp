// This is the part of main calculation. The results of these functions depend on a boundary.
// Calculate the distance between particles.

#include <math.h>
#include <random>
#include "generator.h"
#include "boundary.h"

void initPoints() {
  std::default_random_engine gen;
  gen.seed(seed);
  std::normal_distribution<double> dist(0.0, 5.0);

  for (int i = 0; i < NPOINTS; i++) {
    points[i].x = dist(gen);
    points[i].y = dist(gen);
    points[i].color = (i < NPOINTS / 2) ? RED : BLUE ;
  }
}

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
