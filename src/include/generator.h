#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstring>
#include <map>
#include <vector>

// Constants
#define NPOINTS 50

enum Color {
  RED,
  BLUE,
  BLACK,
  NONE,
};

enum Output {
  HTML,
  JSON,
};

enum Init {
  NORMAL,
  RANDOM,
  SAME,
};

struct Point {
  Point(double x = 0, double y = 0, Color c = NONE)
    : x(x), y(y), color(c) {}

  double x;
  double y;
  Color color;
};


// Global variables.
extern double kparam[NPOINTS][NPOINTS];
extern Point points[NPOINTS];
extern int cycle;
extern double maxk;
extern double mink;
extern std::string dynamic;
extern Init init_param;

// Variables which scope is only this file. 
static std::vector<std::map<double, int> > kparam_counter;
static double initial_kparam[2][2];
static int timestep = 0;
static std::vector<std::vector<Point>> result;
static int seed = 1;
static int maxgen = 50000;
static Output output = HTML;

#endif // GENERATOR_H
