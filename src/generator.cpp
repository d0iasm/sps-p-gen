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

static void initpoints() {
  std::default_random_engine gen;
  gen.seed(seed);
  std::normal_distribution<double> dist(0, 1);

  for (int i = 0; i < npoints; i++) {
    points[i].x = dist(gen);
    points[i].y = dist(gen);
    points[i].color = (i < npoints / 2) ? red : blue;
  }
}

static double rungeKutta(double k1) {
  double k2 = k1 + k1 * 0.002 * 0.5;
  double k3 = k1 + k2 * 0.002 * 0.5;
  double k4 = k1 + k3 * 0.002;
  return (k1 + 2 * k2 + 2 * k3 + k4) * (0.002 / 6.0);
}

static void step() {
  timestep++;
  Point next[NPOINTS];
  Point dxdy[NPOINTS];

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
      x += plsx;
      y += plsy;
    }
    x = rungeKutta(x);
    y = rungeKutta(y);

    next[i] = {imaging(pi.x + x), imaging(pi.y + y), pi.color};
    dxdy[i] = {x, y};
  }
  result.push_back(std::vector<Point>(points, points + NPOINTS));
  memcpy(points, next, sizeof(next));

  // XV.
  xv.push_back(computeXV(dxdy));

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
  </div
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

static void printEnergy() {
  std::cout << "<script>const energy = [\n";
  for (int i = 0; i < energy.size(); i++) {
    std::cout << "[" << i << ", "
              << "{energy_ave:" << energy[0][i] << ", "
              << "energy_var:" << energy[1][i] 
              << "},\n";
  }
  std::cout << "];</script>\n";
}

static void usage() {
  std::cerr << "Usage: generator [ -k1 k00 k01 k10 k11 ] [ -k2 ka kb kp km ] [ -seed number ] [ -gen number ] [ -csv ] [ -csve ]\n";
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

    usage();
  }
}

static void html() {
  std::cout << "<head><link rel=stylesheet href='css/style.css'></head>";
  printBody();
  printPoints();
  printXV();
  printEnergy();
  importScript();
  std::cout << "<script src=open-boundary/script.js></script>\n";
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
            << energy_var() << ","
            << "\n";
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

