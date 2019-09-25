// This is main file. It implements basic algorithm to calcualte next
// step for all particles and output a HTML file embedded particle's X-Y
// coordinate and color as a json format inside a script tag.

#include <iostream>
#include <string>
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
int seed = 0;
double maxk = 1.2;
double mink = -1.2;
std::string dynamic = "none";

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

static std::map<double, int> countKparam() {
  std::map<double, int> m;
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      if (m.find(kparam[i][j]) == m.end())
        m.insert(std::make_pair(kparam[i][j], 1));
      else
        m[kparam[i][j]]++;
    }
  }
  return m;
}

static int countBalance() {
  int count = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        if (kparam[i][j] * kparam[j][i] *
          kparam[i][k] * kparam[k][i] *
          kparam[j][k] * kparam[k][j] >= 0) {
            count++;
        }
      }
    }
  }
  return count;
}

static int countUnbalance() {
  int count = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        if (kparam[i][j] * kparam[j][i] *
          kparam[i][k] * kparam[k][i] *
          kparam[j][k] * kparam[k][j] < 0 ) {
          count++;
        }
      }
    }
  }
  return count;
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
      // TODO: remove  -pow(dist, -2)
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
  std::vector<double> e(4);
  e[0] = energyAverage();
  e[1] = energyVariance();
  e[2] = energyAverageDist();
  e[3] = energyVarianceDist();
  energy.push_back(e);

  // Kparams.
  kparam_counter.push_back(countKparam());
}

static void initPoints() {
  std::default_random_engine gen;
  gen.seed(seed);
  // Default value of cycle:
  // -1 when it's open boundary and result of -1/2 is 0.
  // 10 whne it's periodic boundary.
  std::normal_distribution<double> dist(cycle/2, 2);

  for (int i = 0; i < NPOINTS; i++) {
    points[i].x = dist(gen);
    points[i].y = dist(gen);
    if (init_param == NORMAL)
      points[i].color = (i < NPOINTS / 2) ? RED : BLUE;
    else
      points[i].color = BLACK;
  }
}

static void initKparamWithK() {
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      kparam[i][j] = initial_kparam[i / (NPOINTS / 2)][j / (NPOINTS / 2)];
    }
  }
}

// Initialize K params with [-1.2..1.2]
static void initKparamRandom() {
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      kparam[i][j] = (rand() % 25 - 12) / 10.0;
    }
  }
}

// Initialize K params with 0.0
static void initKparamZero() {
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      kparam[i][j] = 0.0;
    }
  }
}

static double getP() {
  return (initial_kparam[0][1] + initial_kparam[1][0]) / 2;
}

static double getM() {
  return initial_kparam[0][1] - getP();
}

double kAverage() {
  double sum = 0.0;
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      sum += kparam[i][j];
    }
  }
  return sum / (NPOINTS * NPOINTS);
}

double kVariance() {
  double ave = kAverage();
  double sum = 0.0;
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      sum += (kparam[i][j] - ave) * (kparam[i][j] - ave);
    }
  }
  return sum / (NPOINTS * NPOINTS);
}

void printCountedKparam() {
  std::map<double, int> m = countKparam();
  for (std::pair<double, int> e : m)
    outfile << e.first << " => " << e.second << "<br />";
}

static std::string trim(double x, int precision) {
  return std::to_string(x).substr(0, std::to_string(x).find(".") + precision + 1);
}

static std::string filename() {
  // Filename includes the infomation: boundary, cycle, dynamic, maxgen, kparam, and seed.
  // Example: sps-p&b=open&c=-1&d=none&g=100000&k=zero&s=0
  std::string fn = "";
  // Boundary.
  fn.append("b=");
  fn.append(getBoundary());
  // Cycle.
  fn.append("&c=");
  fn.append(std::to_string(cycle));
  // Dynamic.
  fn.append("&d=");
  fn.append(dynamic);
  // The number of generations.
  fn.append("&g=");
  fn.append(std::to_string(maxgen));
  // Kparams.
  fn.append("&k=");
  if (init_param == NORMAL) {
    fn.append(trim(initial_kparam[0][0], 1));
    fn.append(",");
    fn.append(trim(initial_kparam[1][1], 1));
    fn.append(",");
    fn.append(trim(getP(), 1));
    fn.append(",");
    fn.append(trim(getM(), 1));
  } else if (init_param == RANDOM) {
    fn.append("random");
  } else if (init_param == ZERO) {
    fn.append("zero");
  }
  // Seed.
  fn.append("&s=");
  fn.append(std::to_string(seed));
  return fn;
}

static void printBody() {
  outfile << R"END(<body>
<div class=container>
  <div>
    <canvas id=canvas width=650 height=650></canvas>
    <div class=container>
      <button id=start>Stop</button>
      <button id=reset>Reset</button>
      <button id=relation>ON</button>
    </div>
  </div>
  <div>)END";
  outfile << "<h1>" << filename() << "</h1>";
  outfile << "<br />\n";
  outfile << "<div>Timestep: <input type=text size=6 id=timestep></input></div>\n";
  outfile << "<div>Density: " << getDensity() << "</div>";
  outfile << "<div>Balanced triangles: " << countBalance() << "</div>";
  outfile << "<div>Unbalanced triangles: " << countUnbalance() << "</div>";
  outfile << "<br />\n";
  outfile << "<h2>Static K Parameters</h2>\n";
  outfile << "<div>Initial K[00,01,10,11]: "
            << initial_kparam[0][0] << "," << initial_kparam[0][1] << ","
            << initial_kparam[1][0] << "," << initial_kparam[1][1]
            << "</div>\n";
  outfile << "<div>Initial K[a, b, p, m]: "
            << initial_kparam[0][0] << "," << initial_kparam[1][1] << ","
            << getP() << "," << getM();
  outfile << "</div>\n<br />\n<div>";
  outfile << "<h2>Dynamic K Parameters</h2>\n"
	    << "<span> (minK: " << mink << ", maxK: " << maxk << ")</span><br />"
	    << "Average: " << kAverage() << "<br />"
	    << "Variance: " << kVariance() << "<br />";
  printCountedKparam();
  outfile << "<div><img width=350 src=\"img/kparam%3F" << filename() << ".png\" /></div>";
  outfile << "</div>";
  outfile << "<br />";
  outfile << "<h2>Static Energy</h2>"
            << "Average: " << energy[0][0] << " => " << energy[maxgen-1][0] << "<br />"
            << "Variance: " << energy[0][1] << " => " << energy[maxgen-1][1]
            << "<div><img width=350 src=\"img/static_energy%3F" << filename() << ".png\" /></div>";
  outfile << "<br />";
  outfile << "<h2>Dynamic Energy</h2>"
            << "<div><img width=350 src=\"img/dynamic_energy%3F" << filename() << ".png\" /></div>";
  outfile << "<br />";
  outfile << "<h2>X-V Log Log Plot</h2>"
            << "<div><img width=350 src=\"img/xv%3F" << filename() << ".png\" /></div>";
  outfile << "</div></div>";
}

static void printPoints() {
  outfile << "<script>const points = [\n";
  for (int i = 0; i < result.size(); i += 100) {
    outfile << "  [" << i << ",";
    for (int j = 0; j < result[i].size(); j++) {
      Point &p = result[i][j];
      outfile << "{x:" << p.x
                << ",y:" << p.y
                << ",color:" << p.color;
      outfile << ",k:[";
      for (int k = 0; k < NPOINTS; k++) {
        outfile << kparam_result[i][j][k] << ",";
      }
      outfile << "]},";
    }
    outfile << "],\n";
  }
  outfile << "];</script>\n";
}

static void printKparam() {
  outfile << "<script>const kparam = [\n";
  for (int i = 0; i < kparam_result.size(); i += 100) {
    outfile << "{step: " << i;
    outfile << ",k:[";
    for (int j = 0; j < kparam_result[i].size(); j++) {
      outfile << "[";
      for (int k = 0; k < kparam_result[i][j].size(); k++) {
        outfile << trim(kparam_result[i][j][k], 1) << ",";
      }
      outfile << "],\n";
    }
    outfile << "]},\n";
  }
  outfile << "];</script>\n";
}

static void usage() {
  std::cerr << "Usage: generator [ -k1 k00 k01 k10 k11 ] [ -k2 ka kb kp km ] ";
  std::cerr << "[ -gen number ] [ -cycle number ] [ -seed number ] ";
  std::cerr << "[ -dynamic static/dynamic/local]";
  std::cerr << "[ -init random/zero ]";
  std::cerr << "[ -json ]\n\n";

  std::cerr << "-k1        K paramters. k01 means the degree how the type 0 particle likes the type 1 particle.\n";
  std::cerr << "-k2        K paramters. k00=ka, k01=kp+km, k10=kp-km, and k11=kb.\n";
  std::cerr << "-gen       The number of maximum steps.\n";
  std::cerr << "-cycle     The length of periodic boundary. It is useless for open boundary.\n";
  std::cerr << "-seed      The seed number to be used for generating random number. Default value is 1.\n";
  std::cerr << "-dynamic   The flag to change the K parameters dinamicallybased on static energy/dynamic energy/local static energy. K params are not updated if you omit thid flag.\n";
  std::cerr << "-init      The initial state for all particles. -init random indicates that all particles starts with a random K parameter. -init zero indicates that all particles starts with 0.";
  std::cerr << "-json      Output a json file for creating images by utils.\n";
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
      if (argc < 2)
        usage();
      dynamic = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-init", argv[0]) == 0) {
      if (argc < 2)
        usage();
      if (strcmp(argv[1], "random") == 0)
        init_param = RANDOM;
      if (strcmp(argv[1], "zero") == 0)
        init_param = ZERO;
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-json", argv[0]) == 0) {
      output = JSON;
      argc -= 1;
      argv += 1;
      continue;
    }

    if (strcmp("-head"), argv[0] == 0) {
      // TODO: implement for getting a filename header.
    }

    usage();
  }
}

static void html() {
  outfile << "<head><link rel=stylesheet href='css/style.css'></head>";
  printBody();
  printPoints();
  outfile << "<script>const cycle=" << getCycle() << "</script>\n";
  outfile << getScript() << "\n";
  outfile << "</body></html>\n";
}

static void json() {
  outfile << "["; // Start of Json.
  for (int i = 0; i < maxgen; i+=100) {
    outfile << "{"; // Start of one step.
    // Kparams.
    // Initial Kparams.
    outfile << "\"k\":{";
    outfile << "\"initial\":{";
    outfile << "\"00\":" << initial_kparam[0][0] << ",";
    outfile << "\"01\":" << initial_kparam[0][1] << ",";
    outfile << "\"10\":" << initial_kparam[1][0] << ",";
    outfile << "\"11\":" << initial_kparam[1][1] << ",";
    outfile << "\"a\":" << initial_kparam[0][0] << ",";
    outfile << "\"b\":" << initial_kparam[1][1] << ",";
    outfile << "\"p\":" << getP() << ",";
    outfile << "\"m\":" << getM();
    outfile << "},\n"; // End of k.initial.
    // All Kparams.
    outfile << "\"all\":[";
    for (int j = 0; j < NPOINTS; j++) {
      outfile << "[";
      for (int k = 0; k < NPOINTS; k++) {
        outfile << kparam_result[i][j][k];
        if (k != NPOINTS - 1)
          outfile << ",";
        }
      outfile << "]";
      if (j != NPOINTS - 1)
        outfile << ",\n";
    }
    outfile << "],\n"; // End of k.all.
    // Counting Kparams.
    outfile << "\"count\":[";
    int n = kparam_counter[i].size();
    for (std::pair<double, int> e : kparam_counter[i]) {
      outfile << "[" << e.first << "," << e.second << "]";
      if (n > 1) {
        outfile << ",";
        n--;
      }
    }
    outfile << "]"; // End of k.count.
    outfile << "},\n"; // End of k.
    // Energy.
    outfile << "\"energy\":{";
    outfile << "\"static\":{";
    outfile << "\"average\":" << energy[i][0] << ",";
    outfile << "\"variance\":" << energy[i][1];
    outfile << "},"; // End of energy.static.
    outfile << "\"dynamic\":{";
    outfile << "\"average\":" << energy[i][2] << ",";
    outfile << "\"variance\":" << energy[i][3];
    outfile << "}"; // End of energy.dynamic.
    outfile << "},\n"; // End of energy.
    // XV.
    outfile << "\"xv\":{";
    outfile << "\"x\":" << xv[i].x << ",";
    outfile << "\"v\":" << xv[i].v;
    outfile << "}"; // End of xv.
    outfile << "}\n"; // End of one step.
    if (i < maxgen - 100)
      outfile << ",";
  }
  outfile << "]"; // End on Json.
}

static void storeKparam() {
  std::vector<std::vector<double> > k(NPOINTS);
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      k[i].push_back(kparam[i][j]);
    }
  }
  kparam_result.push_back(k);
}

int main(int argc, char **argv) {
  parseArgs(argc - 1, argv + 1);
  switch (output) {
    case HTML:
      outfile.open(filename() + ".html");
      break;
    case JSON:
      outfile.open(filename() + ".json");
      break;
  }

  switch (init_param) {
    case NORMAL:
      initKparamWithK();
      break;
    case RANDOM:
      initKparamRandom();
      break;
    case ZERO:
      initKparamZero();
      break;
  }

  initPoints();
  center = computeCenter();

  for (int i = 0; i < maxgen; i++) {
    // Store the current Kparam before update it.
    storeKparam();
    if (dynamic.compare("none") != 0) {
        updateKparam();
    }
    step();
  }

  switch (output) {
    case HTML:
      html();
      break;
    case JSON:
      json();
      break;
  }
  outfile.close();
}
