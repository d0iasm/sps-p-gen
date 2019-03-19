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
double kparam[][2] = {{0, 0}, {0, 0}};
Point points[NPOINTS];

static int timestep = 0;
static std::vector<std::vector<Point>> result;
static int seed = 1;
static int maxgen = 50000;

static bool outhtml = true;

// Graph
static std::vector<XV> xv;
static Point center;

Color getColor(int i) {
  return points[i].color;
}

// Calculate an energy for a triangle based on Heider Balance theory. 
double heider(int pi, int pj, int pk) {
  int a = getColor(pi);
  int b = getColor(pj);
  int c = getColor(pk);

  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  double p1 = kparam[a][b] * kparam[b][c] * kparam[a][c];
  double p2 = kparam[a][c] * kparam[c][b] * kparam[a][b];
  double p3 = kparam[b][a] * kparam[a][c] * kparam[b][c];
  double p4 = kparam[b][c] * kparam[c][a] * kparam[b][a];
  double p5 = kparam[c][a] * kparam[a][b] * kparam[c][b];
  double p6 = kparam[c][b] * kparam[b][a] * kparam[c][a];

  return (p1 + p2 + p3 + p4 + p5 + p6) / 6;
}

// Calculate an average energy of all triangles.
double energy_ave() {
  int size = 0;
  double sum = 0; 
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        sum += heider(i, j, k);
      }
    }
  }
  return sum / size;
}

// Calculate a variance energy of all triangles.
double energy_var() {
  double average = 0;
  average += kparam[0][0];
  average += kparam[0][1];
  average += kparam[1][0];
  average += kparam[1][1];
  average /= 4;
 
  int size = 0;
  double sum = 0;
  double tmp = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        tmp = heider(i, j, k);
        sum += (tmp - average) * (tmp - average);
      }
    }
  }
  return sum / size;
}

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
  gen.seed(seed);
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

static double getP() {
  return (kparam[0][1] + kparam[1][0]) / 2;
}

static double getM() {
  return kparam[0][1] -getP();
}

static void printBody() {
  std::cout << R"END(<body>
<div class=container>
  <canvas id=canvas width=650 height=650></canvas>
  <div>
    <div class=container>
      <button id=start>Stop</button>
      <button id=reset>Reset</button>
    </div>
    <br />
    <div>timestep: <input type=text size=6 id=timestep></input></div>
    <br />
    <div>)END";
  std::cout << "K[00,01,10,11]: "
            << kparam[0][0] << "," << kparam[0][1] << ","
            << kparam[1][0] << "," << kparam[1][1];
  std::cout << "</div><div>";
  std::cout << " K[a, b, p, m]: " 
            << kparam[0][0] << "," << kparam[1][1] << ","
            << getP() << "," << getM();
  std::cout <<  R"END(</div>
    <br />
    <div>balance energy (average): <span id=energy_ave></span></div>
    <div>balance energy (variance): <span id=energy_var></span></div>
    <br />
    <div>X-V log log plot:</div>
    <canvas id=graph width=250 height=250></canvas>
  </div>
</div>
)END";
}

static void printPoints() {
  std::cout << "<script>const points = [\n";
  for (int i = 0; i < result.size(); i += 100) {
    std::cout << "  [" << i << ",";
    for (Point &p : result[i])
      std::cout << "{x:" << p.x
                << ",y:" << p.y
                << ",color:" << p.color
                << "},";
    std::cout << "],\n";
  }
  std::cout << "];</script>\n";
}

static void printXV() {
  std::cout << "<script>const xv = [\n";
  for (int i = 0; i < xv.size(); i += 100) {
    std::cout << "  [" << i << ", "
              << "{x:" << xv[i].x 
              << ",v:" <<xv[i].v
              << "}],\n";
  }
  std::cout << "];</script>\n";
}

static void usage() {
  std::cerr << "Usage: generator [ -k1 k00 k01 k10 k11 ] [ -k2 ka kb kp km ] [ -seed number ] [ -gen number ]\n";
  exit(1);
}

static void parseArgs(int argc, char **argv) {
  while (argc > 0) {
    if (strcmp("-k1", argv[0]) == 0) {
      if (argc < 5)
        usage();
      kparam[0][0] = std::stod(argv[1]);
      kparam[0][1] = std::stod(argv[2]);
      kparam[1][0] = std::stod(argv[3]);
      kparam[1][1] = std::stod(argv[4]);
      argc -= 5;
      argv += 5;
      continue;
    }

    if (strcmp("-k2", argv[0]) == 0) {
      if (argc < 5)
        usage();
      double a = std::stod(argv[1]);
      double b = std::stod(argv[2]);
      double p = std::stod(argv[3]);
      double m = std::stod(argv[4]);
      kparam[0][0] = a;
      kparam[0][1] = p + m;
      kparam[1][0] = p - m; 
      kparam[1][1] = b; 
      argc -= 5;
      argv += 5;
      continue;
    }

    if (strcmp("-seed", argv[0]) == 0) {
      if (argc < 2)
        usage();
      seed = std::stoi(argv[1]);
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-gen", argv[0]) == 0) {
      if (argc < 2)
        usage();
      maxgen = std::stoi(argv[1]);
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-csv", argv[0]) == 0) {
      outhtml = false;
      argc -= 1;
      argv += 1;
      continue;
    }

    usage();
  }
}

static void html() {
  std::cout << "<head><link rel=stylesheet href=style.css></head>";
  printBody();
  printPoints();
  printXV();
  std::cout << "<script src=script.js></script>\n";
  std::cout << "<script>"
            << "document.getElementById('energy_ave').innerText="
            << energy_ave() << ";" 
            << "document.getElementById('energy_var').innerText="
            << energy_var() << ";" 
            << "</script>\n";
  std::cout << "</body></html>\n";
}

static void csv() {
  std::cout << kparam[0][0] << " " << kparam[0][1] << " " << kparam[1][0] << " " << kparam[1][1] << ","
            << kparam[0][0] << ","
            << kparam[0][1] << ","
            << kparam[1][0] << ","
            << kparam[1][1] << ","
            << kparam[0][0] << " " << kparam[1][1] << " " << getP() << " " << getM() << ","
            << kparam[0][0] << ","
            << kparam[1][1] << ","
            << getP() << ","
            << getM() << ","
            << energy_ave() << ","
            << energy_var() << "\n";
}

int main(int argc, char **argv) {
  parseArgs(argc - 1, argv + 1);

  initPoints();
  for (int i = 0; i < maxgen; i++)
    step();

  if (outhtml) {
    html();  
  } else {
    csv();
  }
}

