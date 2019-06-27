// This is main file. It implements basic algorithm to calcualte next
// step for all particles and output a HTML file embedded particle's X-Y
// coordinate and color as a json format inside a script tag.

#include <iostream>
#include <cstring>
#include <random>
#include <math.h>
#include <map>
#include <vector>
#include "boundary.h"
#include "dynamic.h"
#include "energy.h"
#include "xv.h"
#include "generator.h"

// Global variables.
double kparam[NPOINTS][NPOINTS];
Point points[NPOINTS];
int cycle = 10;

// Global variables declared in xv.h.
Point center;
std::vector<XV> xv;

// Global variables declared in energy.cpp.
std::vector<std::vector<double> > energy;

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
      double k = kparam[i][j];

      if (dist == 0) continue;
      double plsx = (k / dist - pow(dist, -2)) * dx / dist;
      double plsy = (k / dist - pow(dist, -2)) * dy / dist;
      x += plsx;
      y += plsy;
    }
    x = rungeKutta(x);
    y = rungeKutta(y);

    next[i] = {imaging(pi.x + x), imaging(pi.y + y), pi.color};
    dxdy[i] = {imaging(x), imaging(y)};
  }
  result.push_back(std::vector<Point>(points, points + NPOINTS));
  memcpy(points, next, sizeof(next));

  // XV.
  xv.push_back(computeXV(dxdy));

  // Energies.
  std::vector<double> e(2);
  e[0] = energyAverageDist();
  e[1] = energyVarianceDist();
  energy.push_back(e);
}

static void initKparam() {
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      kparam[i][j] = initial_kparam[i / (NPOINTS / 2)][j / (NPOINTS / 2)];
    }
  }
}

static double getP() {
  return (initial_kparam[0][1] + initial_kparam[1][0]) / 2;
}

static double getM() {
  return initial_kparam[0][1] - getP();
}

void debugKparam() {
  std::map<double, int> m;
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      if (m.count(kparam[i][j]) == 0)
        m.insert(std::make_pair(kparam[i][j], 1));
      else
        m[kparam[i][j]]++;
    }
  }
  for (std::pair<double, int> e : m)
    std::cerr << e.first << " => " << e.second << "\n";
}

void printKparam() {
  std::map<double, int> m;
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      if (m.count(kparam[i][j]) == 0)
        m.insert(std::make_pair(kparam[i][j], 1));
      else
        m[kparam[i][j]]++;
    }
  }
  for (std::pair<double, int> e : m)
    std::cout << e.first << " => " << e.second << "<br />";
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
  std::cout << "Initial K[00,01,10,11]: "
            << initial_kparam[0][0] << "," << initial_kparam[0][1] << ","
            << initial_kparam[1][0] << "," << initial_kparam[1][1];
  std::cout << "</div><div>";
  std::cout << "Initial K[a, b, p, m]: " 
            << initial_kparam[0][0] << "," << initial_kparam[1][1] << ","
            << getP() << "," << getM();
  std::cout << "</div><div>";
  std::cout << "Kparam => The number of particles <br />";
  printKparam();
  std::cout <<  R"END(</div>
    <br />
    <div>balance energy (average): <span id=energyAverage></span></div>
    <div>balance energy (variance): <span id=energyVariance></span></div>
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
              << "{energyAverage:" << energy[0][i] << ", "
              << "energyVariance:" << energy[1][i] 
              << "},\n";
  }
  std::cout << "];</script>\n";
}

static void usage() {
  std::cerr << "Usage: generator [ -k1 k00 k01 k10 k11 ] [ -k2 ka kb kp km ] ";
  std::cerr << "[ -gen number ] [ -cycle number ] [ -seed number ] ";
  std::cerr << "[ -dynamic ] ";
  std::cerr << "[ -csv ] [ -csve ]\n\n";

  std::cerr << "-k1        K paramters. k01 means the degree how the type 0 particle likes the type 1 particle.\n";
  std::cerr << "-k2        K paramters. k00=ka, k01=kp+km, k10=kp-km, and k11=kb.\n";
  std::cerr << "-gen       The number of maximum steps.\n";
  std::cerr << "-cycle     The length of periodic boundary. It is useless for open boundary.\n";
  std::cerr << "-seed      The seed number to be used for generating random number. Default value is 1.\n";
  std::cerr << "-dynamic   The flag to change the K parameters dinamically. Default is off.\n";
  std::cerr << "-csv       Output a csv file.\n";
  std::cerr << "-csve      Output csv files for each step.\n";
  exit(1);
}

static void parseArgs(int argc, char **argv) {
  while (argc > 0) {
    if (strcmp("-k1", argv[0]) == 0) {
      if (argc < 5)
        usage();
      initial_kparam[0][0] = std::stod(argv[1]);
      initial_kparam[0][1] = std::stod(argv[2]);
      initial_kparam[1][0] = std::stod(argv[3]);
      initial_kparam[1][1] = std::stod(argv[4]);
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
      initial_kparam[0][0] = a;
      initial_kparam[0][1] = p + m;
      initial_kparam[1][0] = p - m; 
      initial_kparam[1][1] = b; 
      argc -= 5;
      argv += 5;
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

    if (strcmp("-cycle", argv[0]) == 0) {
      if (argc < 2)
        usage();
      cycle = std::stoi(argv[1]);
      argc -= 2;
      argv += 2;
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

    if (strcmp("-dynamic", argv[0]) == 0) {
      dynamic = true;
      argc -= 1;
      argv += 1;
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
  printCycle();
  importScript();
  std::cout << "<script>"
            << "document.getElementById('energyAverage').innerText="
            << energyAverage() << ";" 
            << "document.getElementById('energyVariance').innerText="
            << energyVariance() << ";" 
            << "</script>\n";
  std::cout << "</body></html>\n";
}

static void csv() {
  std::cout << initial_kparam[0][0] << ","
            << initial_kparam[0][1] << ","
            << initial_kparam[1][0] << ","
            << initial_kparam[1][1] << ","
            << initial_kparam[0][0] << ","
            << initial_kparam[1][1] << ","
            << getP() << ","
            << getM() << ","
            << classify() << ","
            << energyAverage() << ","
            << energyVariance() << ","
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

  initKparam();
  initPoints();
  center = computeCenter();

  for (int i = 0; i < maxgen; i++) {
    if (dynamic)
      updateKparam();
    step();
  }

  debugKparam();
  std::cerr << "energy average: " << energyAverage() << "\n";
  std::cerr << "energy variance: " << energyVariance() << "\n";

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