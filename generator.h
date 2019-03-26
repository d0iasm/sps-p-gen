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

struct Point {
  Point(double x = 0, double y = 0, Color c = NONE)
    : x(x), y(y), color(c) {}

  double x;
  double y;
  Color color;
};

struct XV {
  double x;
  double v;
};

// Variables
extern double kparam[2][2];
extern Point points[NPOINTS];

static int timestep = 0;
static std::vector<std::vector<Point>> result;
static int seed = 1;
static int maxgen = 50000;

static bool outhtml = true;
static bool interact_all = true;

// Graph
static std::vector<XV> xv;
static Point center;

#endif // GENERATOR_H
