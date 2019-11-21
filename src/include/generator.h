#ifndef GENERATOR_H

#define GENERATOR_H

#include <cstring>
#include <fstream>
#include <map>
#include <vector>
#include "steps.pb.h"

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
  ZERO,
};

struct Point {
  Point(double x = 0, double y = 0, Color c = NONE)
    : x(x), y(y), color(c) {}

  double x;
  double y;
  Color color;
};

// Global function.
std::string filename();

// Global variables.
extern double kparam[NPOINTS][NPOINTS];
extern Point points[NPOINTS];
extern double maxk;
extern double mink;
extern std::string dynamic;
extern std::string p1;
extern std::string p2;

// Variables which scope is only this file.
static data::Steps steps;
static double initial_kparam[2][2];
static int timestep = 0;
static std::vector<std::vector<Point>> point_result;
static std::vector<std::vector<std::vector<double> > > kparam_result;
static std::vector<std::map<double, int> > kparam_counter;
static int seed = 0;
static int maxgen = 50000;
static Output output = HTML;
static Init init_param = NORMAL;
static std::ofstream outfile;
static std::string path_html = ".";
static std::string path_proto = "./proto/bin";
static int thinning = 200;

#endif // GENERATOR_H
