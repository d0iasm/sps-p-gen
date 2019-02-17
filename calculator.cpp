#include <iostream>
#include <cstring>
#include <random>
#include <math.h>

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
static double K[][2] = {{0, 0}, {0, 0}};
static int timestep = 0;
static Point points[NPOINTS];

// Graph
static std::vector<XV> xv;
static Point center;

static Point computeCenter() {
  double x = 0;
  double y = 0;
  for (Point &p : points) {
    x += p.x;
    y += p.y;
  }
  return {x, y};
}

static void initPoints() {
  std::default_random_engine gen;
  std::normal_distribution<double> dist(0, 1);

  for (int i = 0; i < NPOINTS; i++) {
    points[i].x = dist(gen);
    points[i].y = dist(gen);
    points[i].color = (i < NPOINTS / 2) ? RED : BLUE;
  }
  center = computeCenter();
}

static double rungeKutta(double k1) {
  double k2 = k1 + k1 * 0.002 * 0.5;
  double k3 = k1 + k2 * 0.002 * 0.5;
  double k4 = k1 + k3 * 0.002;
  return (k1 + 2 * k2 + 2 * k3 + k4) * (0.002 / 6.0);
}

static double distance(Point p, Point q) {
  double dx = p.x - q.x;
  double dy = p.y - q.y;
  return sqrt(dx * dx + dy * dy);
}

static void computeXV(Point *delta) {
  // Compute values for drawing the graph. First, compute X.
  double sum = 0;
  for (Point &p : points) {
    sum += distance(center, p);
  }
  double x = NPOINTS / sum;

  // Compute V.
  Point newCenter = computeCenter();
  double cx = newCenter.x - center.x;
  double cy = newCenter.y - center.y;
  sum = 0;
  for (int i = 0; i < NPOINTS; i++) {
    Point &d = delta[i];
    double dx = d.x - cx;
    double dy = d.y - cy;
    sum += sqrt(dx * dx + dy * dy);
  }
  double v = sum / NPOINTS;

  center = newCenter;
  xv.push_back({x, v});
}

static void step() {
  timestep++;
  Point ps[NPOINTS];
  Point delta[NPOINTS];

  for (int i = 0; i < NPOINTS; i++) {
    Point &pi = points[i];
    double x = 0;
    double y = 0;

    for (int j = 0; j < NPOINTS; j++) {
      if (i == j)
        continue;
      Point &pj = points[j];

      double dx = pj.x - pi.x;
      double dy = pj.y - pi.y;
      double dist = sqrt(dx * dx + dy * dy);
      double k = K[pi.color][pj.color];
      x += (k / dist - pow(dist, -2)) * dx / dist;
      y += (k / dist - pow(dist, -2)) * dy / dist;
    }
    x = rungeKutta(x);
    y = rungeKutta(y);

    ps[i] = {pi.x + x, pi.y + y, pi.color};
    delta[i] = {x, y};
  }
  memcpy(points, ps, sizeof(ps));
  computeXV(delta);
}

static void print() {
  std::cout << timestep << ",";
  for (Point &p : points)
    std::cout << p.x << "," << p.y << "," << p.color << ",";
  std::cout << "\n";
}

int main() {
  initPoints();

  for (int i = 0; i < 100000; i++) {
    step();
    if (i % 200 == 0)
      print();
  }
}
