// This file implements basic algorithm to calcualte next step for all
// particles and output a HTML file embedded particle's X-Y coordinate
// and color as a json format inside a script tag.

#include <iostream>
#include <cstring>
#include <random>
#include <math.h>
#include <vector>
#include "energy.h"
#include "generator.h"
#include "xv.h"

// Global variables.
double kparam[2][2];
Point points[NPOINTS];

// Global variables declared in xv.cpp.
Point center;
std::vector<XV> xv;

// Global variables declared in energy.cpp.
std::vector<std::vector<double> > energy;

static double distanceDirect(double x1, double y1, double x2, double y2) {
  return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double rem(double x, long y) {
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
static double diff(double a, double b) {
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

static void initPoints() {
  std::default_random_engine gen;
  gen.seed(seed);
  std::normal_distribution<double> dist(5.0, 2.0);

  for (int i = 0; i < NPOINTS; i++) {
    points[i].x = dist(gen);
    points[i].y = dist(gen);
    points[i].color = (i < NPOINTS / 2) ? RED : BLUE;
  }
}

// Adjust the posision from -|cycle/2| to |cycle/2|. 
static double imaging(double x) {
  if (x < 0) return rem(x, cycle) + cycle;
  if (x > cycle) return rem(x, cycle);
  return x;
}

static double rungeKutta(double k1) {
  double k2 = k1 + k1 * 0.002 * 0.5;
  double k3 = k1 + k2 * 0.002 * 0.5;
  double k4 = k1 + k3 * 0.002;
  return (k1 + 2 * k2 + 2 * k3 + k4) * (0.002 / 6.0);
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

      double dx = diff(pi.x, pj.x);
      double dy = diff(pi.y, pj.y);
      double dist = distance(pi, pj);
      double k = kparam[pi.color][pj.color];

      if (dist == 0) continue;
      double plsx = (k / dist - pow(dist, -2)) * dx / dist;
      double plsy = (k / dist - pow(dist, -2)) * dy / dist;
      if (!interact_all) {
        plsx = plsx / dist;
        plsy = plsy / dist;
      } 
      x += plsx;
      y += plsy;
    }
    x = rungeKutta(x);
    y = rungeKutta(y);
    ps[i] = {imaging(pi.x + x), imaging(pi.y + y), pi.color};
    delta[i] = {x, y};
  }
  result.push_back(std::vector<Point>(points, points + NPOINTS));
  memcpy(points, ps, sizeof(ps));

  // XV.
  xv.push_back(computeXV(delta));

  // Energies.
  std::vector<double> e(2);
  e[0] = energy_ave_dist();
  e[1] = energy_var_dist();
  energy.push_back(e);
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
    <h1>SPS-P: Periodic Boundary</h1>
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
    <canvas id=graphXV width=250 height=250></canvas>
    <!--
    <div>Variance Energy:</div>
    <canvas id=graphEnergy width=250 height=250></canvas>
    -->
  </div>
</div>
)END";
}

static void printPoints() {
  std::cout << "<script>const points = [\n";
  for (int i = 0; i < result.size(); i += thinning) {
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
  for (int i = 0; i < xv.size(); i += thinning) {
    std::cout << "  [" << i << ", "
              << "{x:" << xv[i].x 
              << ",v:" <<xv[i].v
              << "}],\n";
  }
  std::cout << "];</script>\n";
}

static void printEnergy() {
  std::cout << "<script>const energy = [\n";
  for (int i = 0; i < energy.size(); i++) {
    std::cout << " [" << i << ", "
              << "{energy_ave:" << energy[0][i] << ", "
              << "energy_var:" << energy[1][i] 
              << "}],\n";
  }
  std::cout << "];</script>\n";
}

static void usage() {
  std::cerr << "Usage: generator [ -k1 k00 k01 k10 k11 ] [ -k2 ka kb kp km ] [ -seed number ] [ -gen number ] [ -csv ] [-csve] [ -dist ]\n";
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
      output = CSV;
      argc -= 1;
      argv += 1;
      continue;
    }

    if (strcmp("-csve", argv[0]) == 0) {
      output = CSVE;
      argc -= 1;
      argv += 1;
      continue;
    }

    if (strcmp("-dist", argv[0]) == 0) {
      interact_all = false;
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
  //printEnergy();
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
  std::cout << kparam[0][0] << ","
            << kparam[0][1] << ","
            << kparam[1][0] << ","
            << kparam[1][1] << ","
            << kparam[0][0] << ","
            << kparam[1][1] << ","
            << getP() << ","
            << getM() << ","
            << classify() << ","
            << energy_ave() << ","
            << energy_var() << "\n";
}

static void csve() {
  std::cout << "step,energy-average,energy-variance\n";
  for (int i = 0; i < energy.size(); i++) {
    std::cout << i << "," << energy[i][0] << "," << energy[i][1] << "\n"; 
  }
}

int main(int argc, char **argv) {
  parseArgs(argc - 1, argv + 1);

  initPoints();
  center = computeCenter();

  for (int i = 0; i < maxgen; i++)
    step();

  switch (output) {
    case HTML:
      html();
      break;
    case CSV:
      csv();
      break;
    case CSVE:
      csve();
      break;
  }
}

