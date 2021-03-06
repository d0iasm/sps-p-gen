// Calculate the nearest distance between particles based on
// This is the part of main calculation. The results of these functions depend on a boundary.
// Periodic boundary uses the nearest particle selection method.

#include <cstring>
#include <random>
#include <math.h>
#include "generator.h"
#include "boundary.h"

// Global variable defined in boundary.h.
// The value 10 is default. It can be rewritten by `-cycle` flag.
int cycle = 10;

static double rem(double x, long y) {
    return (long)x % y + (x - (long)x);
}

// Nearest particle selection method.
double distance(Point p, Point q) {
  double closest = (double) 2147483647; // Instead of INT_MAX.
  double x1 = rem(p.x, cycle);
  double y1 = rem(p.y, cycle);
  double x2 = rem(q.x, cycle);
  double y2 = rem(q.y, cycle);

  for (int i=-1; i<=1; i++) {
    for (int j=-1; j<=1; j++) {
      double dx = (x2 + cycle * i) - x1;
      double dy = (y2 + cycle * j) - y1;
      double tmp = sqrt(dx * dx + dy * dy);
      if (tmp < closest)
        closest = tmp;
    }
  }
  return closest;
}

// Nearest particle selection method.
// The direction is always a->b.
double diff(double a, double b) {
  double closest = b - a;
  a = rem(a, cycle);
  b = rem(b, cycle);
  for (int i=-1; i<=1; i++) {
    double tmp = (cycle * i + b) - a;
    if (abs(tmp) < abs(closest))
      closest = tmp;
  }
  return closest;
}

// Adjust the posision from -|cycle/2| to |cycle/2|.
double imaging(double x) {
  if (x < 0) return rem(x, cycle) + cycle;
  if (x > cycle) return rem(x, cycle);
  return x;
}

std::string getBoundary() {
  return "periodic";
}

int getCycle() {
  return cycle;
}

double getDensity() {
  return (double) NPOINTS / (double) (cycle * cycle);
}

std::string getScript() {
  return "<script src=js/periodic-boundary/script.js></script>";
}
