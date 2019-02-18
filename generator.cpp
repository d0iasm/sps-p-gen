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
static int seed = 1;
static int maxgen = 50000;

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

static void printHeader() {
  std::cout << R"END(<html>
<body>
<canvas id=canvas width=650 height=650></canvas>
<span>)END";
  std::cout << "K="
            << kparam[0][0] << "," << kparam[0][1] << ","
            << kparam[1][0] << "," << kparam[1][1];
  std::cout << " K[a,b,p,m]=" 
            << kparam[0][0] << "," << kparam[1][1] << ","
            << getP() << "," << getM();
  std::cout <<  R"END(</span>
timestep: <span id=timestep></span>
<button id=start>Stop</button>
<button id=reset>Reset</button>
<canvas id=graph width=200 height=200></canvas>
<script>
)END";
}

static void printFooter() {
  std::cout << R"END(</script>
</body>
</html>
)END";
}

static void printScript() {
  std::cout << R"END(
// Main canvas
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext('2d');
ctx.scale(0.65, 0.65);
ctx.translate(500, 500);

// Log log graph
const graph = document.getElementById("graph");
const gctx = graph.getContext('2d');
gctx.scale(200, 200);

// HTML elements
const startButton = document.getElementById('start');
const resetButton = document.getElementById('reset');

// Constants
const RED = 0;
const BLUE = 1;

// Variables
let currentScale = 1;
let handle;
let index = 0;

function drawGrid() {
  ctx.save();
  ctx.strokeStyle = '#aaa';
  ctx.lineWidth = 1 / currentScale;

  const len = 5000;
  for (let x = -len; x < len; x += 10) {
    ctx.beginPath();
    ctx.moveTo(x, -len);
    ctx.lineTo(x, len);
    ctx.stroke();

    ctx.beginPath();
    ctx.moveTo(-len, x);
    ctx.lineTo(len, x);
    ctx.stroke();
  }
  ctx.restore();
}

function drawPoint(p) {
  ctx.save();
  ctx.fillStyle = (p.color == RED) ? 'red' : 'blue';
  ctx.beginPath();
  ctx.arc(p.x, p.y, 5 / currentScale, 0, 2 * Math.PI, true);
  ctx.fill();
  ctx.restore();
}

function scaleout() {
  let max = 0;
  for (let i = 1; i < points[index].length; i++) {
    const p = points[index][i];
    max = Math.max(max, Math.abs(p.x), Math.abs(p.y));
  }

  currentScale = 500 / (max * 1.2);
  ctx.scale(currentScale, currentScale);
}

function chaseGraph() {
  gctx.save();
  gctx.translate(0.25, 0.75);
  gctx.scale(0.25, -0.25);

  gctx.fillStyle = 'red';

  const p = xv[index][1];
  const v = Math.log10(1000 * p.v + 1);
  const x = Math.log10(1000 * p.x + 1);

  gctx.beginPath();
  gctx.arc(x, v, 0.03, 0, 2 * Math.PI, true);
  gctx.fill();

  gctx.restore();
}

function drawGraph() {
  gctx.save();
  gctx.translate(0.25, 0.75);
  gctx.scale(0.25, -0.25);

  gctx.fillStyle = 'blue';

  for (let i = 0; i < xv.length; i++) {
    const p = xv[i][1];
    const v = Math.log10(1000 * p.v + 1);
    const x = Math.log10(1000 * p.x + 1);

    gctx.beginPath();
    gctx.arc(x, v, 0.03, 0, 2 * Math.PI, true);
    gctx.fill();
  }

  gctx.restore();
}

function drawGraphGrid() {
  gctx.clearRect(0, 0, 1, 1);

  gctx.save();
  gctx.strokeStyle = '#ccc';
  gctx.lineWidth = 0.005;

  for (let x = 0.25; x < 1; x += 0.25) {
    gctx.beginPath();
    gctx.moveTo(x, 0);
    gctx.lineTo(x, 1);
    gctx.stroke();

    gctx.beginPath();
    gctx.moveTo(0, x);
    gctx.lineTo(1, x);
    gctx.stroke();
  }

  gctx.lineWidth = 0.02;
  gctx.beginPath();
  gctx.moveTo(0.25, 0);
  gctx.lineTo(0.25, 1);
  gctx.stroke();

  gctx.beginPath();
  gctx.moveTo(0, 0.75);
  gctx.lineTo(1, 0.75);
  gctx.stroke();
  gctx.restore();

  gctx.save();
  gctx.scale(.005, .005);
  gctx.fillText('log10(10^3*V+1)', 5, 10);
  gctx.fillText('log10(10^3*X+1)', 120, 165);
  gctx.restore();
}

function redraw() {
  ctx.save();
  ctx.clearRect(-50000, -50000, 100000, 100000);
 
  chaseGraph();

  scaleout();
  drawGrid();

  for (let i = 1; i < points[index].length; i++) {
    drawPoint(points[index][i]);
  }

  document.getElementById('timestep').innerText = points[index][0];
  ctx.restore();
}

function step() {
  if (index < points.length) {
    redraw();
    index++;
  } else {
    stop();
  }
}

function start() {
  handle = window.setInterval(step, 0);
  startButton.innerText = 'Stop';
}

function stop() {
  window.clearInterval(handle);
  handle = undefined;
  startButton.innerText = 'Start';
}

function reset() {
  const running = handle;
  if (running) stop();
  index = 0;
  redraw();
  if (running) start();
}

startButton.addEventListener('click', function() {
  if (handle) {
    stop();
  } else {
    start();
  }
});

resetButton.addEventListener('click', reset);

drawGraphGrid();
drawGraph();
start();
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
              << "{x:" << xv[i].x 
              << ",v:" <<xv[i].v
              << "}],\n";
  }
  std::cout << "];\n";
}

static void usage() {
  std::cerr << "Usage: generator [ -k k0 k1 k2 k3 ] [ -kx ka kb kp km ] [ -seed number ] [ -gen number ]\n";
  exit(1);
}

static void parseArgs(int argc, char **argv) {
  while (argc > 0) {
    if (strcmp("-k", argv[0]) == 0) {
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

    if (strcmp("-kx", argv[0]) == 0) {
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

    usage();
  }
}

int main(int argc, char **argv) {
  parseArgs(argc - 1, argv + 1);

  initPoints();
  for (int i = 0; i < maxgen; i++)
    step();

  printHeader();
  printPoints();
  printXV();
  printScript();
  printFooter();
}

