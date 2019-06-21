#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>

// Constants
#define NPOINTS 50

enum Color {
  RED,
  BLUE,
  NONE,
};

enum Output {
  HTML,
  CSV,
  CSVE,
};

struct Point {
  Point(double x = 0, double y = 0, Color c = NONE)
    : x(x), y(y), color(c) {}

  double x;
  double y;
  Color color;
};

// Global variables.
extern double kparam[2][2];
extern Point points[NPOINTS];

// Variables which scope is only this file. 
static int timestep = 0;
static std::vector<std::vector<Point>> result;
static int seed = 1;
static int maxgen = 50000;
static Output output = HTML;

// Variable which is specific for periodic boundary.
static int cycle = 10;

#endif // GENERATOR_H
