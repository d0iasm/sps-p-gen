// This is the part of main calculation. The results of these functions depend on a boundary.
// Calculate the distance between particles.

#include <string>
#include <math.h>
#include <random>
#include "generator.h"
#include "boundary.h"

// Global variable defined in boundary.h.
int cycle = -1;

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

double density() {
  return -1;
}

std::string boundary() {
  return "open";
}
