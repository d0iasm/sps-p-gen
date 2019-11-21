// This is main file. It implements basic algorithm to calcualte next
// step for all particles and output a HTML file, a JS file to include
// particle's X-Y coordinate and k parameters and a protobol buffer binary
// to convey data to other script like Python.

#include <iostream>
#include <string>
#include <random>
#include <regex>
#include <math.h>
#include <map>
#include <vector>
#include "boundary.h"
#include "dynamic.h"
#include "energy.h"
#include "gen_js.h"
#include "xv.h"
#include "generator.h"
#include "steps.pb.h"

// Global variables.
double kparam[NPOINTS][NPOINTS];
Point points[NPOINTS];
double maxk = 1.0;
double mink = -1.0;
std::string dynamic = "none";
std::string p1 = "0";
std::string p2 = "0";

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

// Count balanced triangles based on average of asymmetrical edges.
static int countBalanceAverage() {
  int count = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        if (((kparam[i][j] + kparam[j][i]) / 2) *
          ((kparam[i][k] + kparam[k][i]) / 2) *
          ((kparam[j][k] + kparam[k][j]) / 2) >= 0) {
            count++;
        }
      }
    }
  }
  return count;
}

// Count balanced triangles based on permutations.
static int countBalanceHeider() {
  int count = 0;
  for (int p =0; p < NPOINTS; p++) {
    for (int o = 0; o < NPOINTS; o++) {
      if (p == o)
        continue;
      for (int x = 0; x < NPOINTS; x++) {
        if (p == x || o == x)
          continue;
        if (kparam[p][o] * kparam[o][x] * kparam[p][x] >= 0) {
          count++;
        }
      }
    }
  }
  return count;
}

static int countUnbalanceAverage() {
  int count = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        if (((kparam[i][j] + kparam[j][i]) / 2) *
          ((kparam[i][k] + kparam[k][i]) / 2) *
          ((kparam[j][k] + kparam[k][j]) / 2) < 0) {
            count++;
        }
      }
    }
  }
  return count;
}

static int countUnbalanceHeider() {
  int count = 0;
  for (int p =0; p < NPOINTS; p++) {
    for (int o = 0; o < NPOINTS; o++) {
      if (p == o)
        continue;
      for (int x = 0; x < NPOINTS; x++) {
        if (p == x || o == x)
          continue;
        if (kparam[p][o] * kparam[o][x] * kparam[p][x] < 0) {
          count++;
        }
      }
    }
  }
  return count;
}

// Noise for K parameters. It is possible to reverse its value with p1 % probability.
// Minimum probability is 0.001%.
static double noise_inc = 10.0;
static void noiseP1() {
  for (int i = 0; i < NPOINTS; i++) {
    for (int j = 0; j < NPOINTS; j++) {
      // Incremental noise when p1 is lower than 0.
      if (std::stod(p1) < 0) {
        if ((rand() % 100000) < noise_inc * 1000) {
          kparam[i][j] = (rand() % (int)(maxk*2*10+1) - (int)maxk*10) / 10.0;
        }
        continue;
      }

      // Fixed noise.
      if ((rand() % 100000) < std::stod(p1) * 1000) {
        kparam[i][j] = (rand() % (int)(maxk*2*10+1) - (int)maxk*10) / 10.0;
      }
    }
  }
  noise_inc += 10.0;
}

// Noise for a position from -cycle/10 to cycle/10 (or from -3 to 3 in open boundary) with p2 % probability.
// Minimum probability is 0.001%.
static Point noiseP2() {
  double x = 0;
  double y = 0;
  int base = (cycle == -1) ? 3 : (cycle * 2 / 10 + 1) ;

  if ((rand() % 100000) < std::stod(p2) * 1000) { // p2 * 100000 / 100 because of p2 %.
    x = (rand() % base) - (base / 2);
    y = (rand() % base) - (base / 2);
  }
  // This is difference, not a position for a particle.
  return {x, y, NONE};
}

static void storeStep(XV xv, int step) {
  auto *s = steps.add_steps();
  s->set_step(step);
  for (int i=0; i < NPOINTS; i++) {
    auto *p = s->add_particles();
    p->set_x(points[i].x);
    p->set_y(points[i].y);

    for (int j = 0; j < NPOINTS; j++) {
      p->add_kparams(int(kparam[i][j] * 10));
    }
  }
  s->set_static_energy(energyAverage(0, 0));
  s->set_dynamic_energy(energyAverageDist(0, 0));
  s->set_static_energy_variance(energyVariance());
  s->set_dynamic_energy_variance(energyVarianceDist());
  s->set_x_value(xv.x);
  s->set_v_value(xv.v);
}

// Inject noise to 1 particle per a step.
static void step(int step) {
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

  if (step % thinning == 0) {
    auto xv = computeXV(dxdy);
    storeStep(xv, step);
  }

  memcpy(points, next, sizeof(next));

  if (step % thinning == 0) {
    // Points' positions.
    point_result.push_back(std::vector<Point>(points, points + NPOINTS));

    // XV.
    xv.push_back(computeXV(dxdy));

    // Energies.
    std::vector<double> e(4);
    e[0] = energyAverage(0, 0);
    e[1] = energyVariance();
    e[2] = energyAverageDist(0, 0);
    e[3] = energyVarianceDist();
    energy.push_back(e);

    // Kparams.
    kparam_counter.push_back(countKparam());
  }
}

static void initPoints() {
  for (int i = 0; i < NPOINTS; i++) {
    points[i].x = rand() % std::max(10, cycle);
    points[i].y = rand() % std::max(10, cycle);
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
      kparam[i][j] = (rand() % (int)(maxk*2*10+1) - (int)maxk*10) / 10.0;
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

std::string filename() {
  // Filename includes the infomation: boundary, cycle, dynamic, maxgen, kparam, and seed.
  // Example: b=open&c=-1&d=none&g=100000&k=zero&s=0
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
  // Probabilities.
  fn.append("&p1=");
  fn.append(p1);
  fn.append("&p2=");
  fn.append(p2);
  // Seed.
  fn.append("&s=");
  fn.append(std::to_string(seed));
  return fn;
}

static std::string settings() {
  std::string name = filename();
  return std::regex_replace(name, std::regex("&"), "<br />");
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
  <div style="padding: 0 20px;">)END";
  outfile << "<h1>A particle swarm model</h1><br />";
  outfile << "<h2>Settings</h2>" << settings();
  outfile << "<br /><br />\n";
  outfile << "<div>timestep: <input type=text size=6 id=timestep></input></div>\n";
  outfile << "<div>density: " << getDensity() << "</div>";
  outfile << "<div>balanced triangles (average, nC3): " << countBalanceAverage() << "</div>";
  outfile << "<div>unbalanced triangles (average, nC3): " << countUnbalanceAverage() << "</div>";
  outfile << "<div>balanced triangles (heider, nP3): " << countBalanceHeider() << "</div>";
  outfile << "<div>unbalanced triangles (heider, nP3): " << countUnbalanceHeider() << "</div>";
  outfile << "<br />\n";
  outfile << "<h2>Initial K Parameters</h2>"
          << "(valid values only for Kano's parameters)\n";
  outfile << "<div>initial K[00,01,10,11]: "
          << initial_kparam[0][0] << "," << initial_kparam[0][1] << ","
          << initial_kparam[1][0] << "," << initial_kparam[1][1]
          << "</div>\n";
  outfile << "<div>initial K[a, b, p, m]: "
          << initial_kparam[0][0] << "," << initial_kparam[1][1] << ","
          << getP() << "," << getM();
  outfile << "</div>\n<br />\n<div>";
  outfile << "<h2>K Parameters' distribution</h2>\n"
          << "<span> (minK: " << mink << ", maxK: " << maxk << ")</span><br />"
          << "last step (average): " << kAverage() << "<br />"
          << "last step (variance): " << kVariance() << "<br />";
  printCountedKparam();
  outfile << "</div></div><div><h1>Figures</h1>\n<h2>K Parameters</h2>";
  outfile << "<div><img width=350 src=\"img/kparam_" << filename() << ".png\" /></div>";
  outfile << "<br />";
  outfile << "<h2>Static Heider Energy</h2>"
          << "(high: " << energyHigh() << ", low: "<< energyLow() << ")" << "<br />"
          << "initial => final step: " << energy[0][0] << " => " << energy[energy.size()-1][0] << "<br />"
          << "initial => final step (variance): " << energy[0][1] << " => " << energy[energy.size()-1][1]
          << "<div><img width=350 src=\"img/static_energy_" << filename() << ".png\" /></div>";
  outfile << "<br />";
  outfile << "<h2>Dynamic Heider Energy</h2>"
          << "<div><img width=350 src=\"img/dynamic_energy_" << filename() << ".png\" /></div>";
  /*
  outfile << "<br />";
  outfile << "<h2>X-V Log Log Plot</h2>"
          << "<div><img width=350 src=\"img/xv_" << filename() << ".png\" /></div>";
          */
  outfile << "</div></div></div>";
  // clustering
  outfile << "<div class=container>";
  int s[10] = {300000, 400000, 500000, 600000, 700000, 800000, 1000000, 1200000, 1400000, 1600000};
  for (int i=0; i<10; i++) {
    outfile << "<img width=350 src=\"img/clustering_" << filename() << "&step=" << s[i] << ".png\" />";
  }
  outfile << "</div>";
}

static void usage() {
  std::cerr << "Usage: generator [ -k1 k00 k01 k10 k11 ] [ -k2 ka kb kp km ]\n";
  std::cerr << "[ -init random/zero ]\n";
  std::cerr << "[ -gen number ] [ -cycle number ] [ -seed number ]\n";
  std::cerr << "[ -dynamic none/global-static-discrete/global-dynamic-discrete/local-static-discrete/local-dynamic-discrete/local-static-continuous/local-dynamic-continuous]\n";
  std::cerr << "[ -path_html string ] [ -path_proto string ]\n";
  std::cerr << "[ -p1 double ] [ -p2 double ]";
  std::cerr << "\n\n";

  std::cerr << "-k1         K paramters. k01 means the degree how the type 0 particle likes the type 1 particle.\n";
  std::cerr << "-k2         K paramters. k00=ka, k01=kp+km, k10=kp-km, and k11=kb.\n";
  std::cerr << "-init       The initial state for all particles. -init random indicates that all particles starts with a random K parameter. -init zero indicates that all particles starts with 0.\n";
  std::cerr << "-gen        The number of maximum steps.\n";
  std::cerr << "-cycle      The length of periodic boundary. It is useless for open boundary.\n";
  std::cerr << "-seed       The seed number to be used for generating random number. Default value is 1.\n";
  std::cerr << "-dynamic    The flag to change the K parameters dinamically based on static energy/dynamic energy/local static energy. K params are not updated if you omit thid flag.\n";
  std::cerr << "-path_html       The path directory for a html file name.\n";
  std::cerr << "-path_proto The path directory for a protocol buffer binary.\n";
  std::cerr << "-p1         The probability of noise for updateing K params.\n";
  std::cerr << "-p2         The probability of noise for updateing paricle's positions.\n";
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

    if (strcmp("-path_html", argv[0]) == 0) {
      if (argc < 2)
        usage();
      path_html = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-path_proto", argv[0]) == 0) {
      if (argc < 2)
        usage();
      path_proto = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-p1", argv[0]) == 0) {
      if (argc < 2)
        usage();
      p1 = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    if (strcmp("-p2", argv[0]) == 0) {
      if (argc < 2)
        usage();
      p2 = argv[1];
      argc -= 2;
      argv += 2;
      continue;
    }

    usage();
  }
}

static void html() {
  outfile << "<head><link rel=stylesheet href='css/style.css'></head>";
  printBody();
  outfile << "<script src=\"js/data/" << filename() << ".js\"></script>";
  outfile << "<script>const cycle=" << getCycle() << ";</script>\n";
  outfile << getScript() << "\n";
  outfile << "</body></html>\n";
}

int main(int argc, char **argv) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  parseArgs(argc - 1, argv + 1);

  outfile.open(path_html + "/" + filename() + ".html");

  srand(seed);

  // Initialize particle's positions.
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

  // Main loop.
  for (int i = 0; i < maxgen; i++) {
  // Experiment for the movement of particles.
    if (200000 < i && i < 800000 && i % 100000 == 0) {
      noiseP1();
    }
    if (800000 <= i && i < maxgen - 200000 && i % 200000 == 0) {
      noiseP1();
    }

    // Update K parameters based on the Energy.
    updateKparam();

    // Update particle's positions based on Kano's model.
    step(i);
  }

  js(steps);
  html();

  std::fstream output(path_proto + "/" + filename() + ".bin", std::ios::out | std::ios::trunc | std::ios::binary);
  if (!steps.SerializeToOstream(&output)) {
    std::cerr << "Failed to write a protocol buffer.\n";
  }

  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  std::cerr << "genarated: " << filename() << "\n";
  outfile.close();
}
