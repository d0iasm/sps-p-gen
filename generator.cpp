#include <iostream>
#include <cstring>
#include <random>
#include <math.h>
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
static double kparam[][2] = {{0, 0}, {0, 0}};
static int timestep = 0;
static Point points[NPOINTS];
static std::vector<std::vector<Point>> result;

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
      double k = kparam[pi.color][pj.color];
      x += (k / dist - pow(dist, -2)) * dx / dist;
      y += (k / dist - pow(dist, -2)) * dy / dist;
    }
    x = rungeKutta(x);
    y = rungeKutta(y);

    ps[i] = {pi.x + x, pi.y + y, pi.color};
    delta[i] = {x, y};
  }
  result.push_back(std::vector<Point>(points, points + NPOINTS));
  memcpy(points, ps, sizeof(ps));
  computeXV(delta);
}

static void printHeader() {
  std::cout << R"END(<html>
<body>
<script>
)END";
}

static void printFooter() {
  std::cout << R"END(</script>
</body>
</html>
)END";
}

static void printPoints() {
  std::cout << "const points = [\n";
  for (int i = 0; i < result.size(); i += 100) {
    std::cout << "  [" << i << ",";
    for (Point &p : result[i])
      std::cout << "{x:" << p.x
                << ",y:" << p.y
                << ",color:" << p.color
                << "},";
    std::cout << "],\n";
  }
  std::cout << "];\n";
}

static void printXV() {
  std::cout << "const xv = [\n";
  for (int i = 0; i < xv.size(); i += 100) {
    std::cout << "  [" << i << ", "
              << xv[i].x << ", "
              << xv[i].v << "],\n";
  }
  std::cout << "];\n";
}

int main() {
  initPoints();
  for (int i = 0; i < 100000; i++)
    step();

  printHeader();
  printPoints();
  printXV();
  printFooter();
}

