import sys


def write(filename, content):
    with open(filename, 'w') as f:
        f.write(content)


def gen_html(basename):
    return '''<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <title>SPS-P</title>
    <link rel="stylesheet" href="./style.css">
  </head>

  <body>
    <div class=container>
      <canvas id=canvas width=650 height=650 style='border: 1px solid black'>
      </canvas>

      <div>
        timestep: <span id=timestep></span>
        <br>

        K00 <input id=k0 type=text size=5 value=0.8></input>
        K01 <input id=k1 type=text size=5 value=0.2></input>
        K10 <input id=k2 type=text size=5 value=0.8></input>
        K11 <input id=k3 type=text size=5 value=0.2></input>
        <br>
        Ka&nbsp; <input id=ra type=text size=5 value=0.8></input>
        Kb&nbsp; <input id=rb type=text size=5 value=0.2></input>
        Kp&nbsp; <input id=rp type=text size=5 value=0.8></input>
        Km&nbsp; <input id=rm type=text size=5 value=0.2></input>
        <br>
        <button id=start>Stop</button>
        <button id=reset>Reset</button>

        <h2>X-V log log graph</h2>
        <canvas id=graph width=200 height=200 style='border: 1px solid black'>
        </canvas>
      </div>
    </div>
  </body>
  <script src="./{0}.js"></script>
</html>'''.format(basename)


def gen_script():
    return '''// Main canvas
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
const numPoints = 50;
const RED = 0;
const BLUE = 1;

// Variables
let K = [[0, 0], [0, 0]];
let currentScale = 1;
let timestep = 0;
let points = [];
let handle;

// Graph
let xv = [];
let center;

function initPoints() {
  for (let i = 0; i < numPoints; i++) {
    const color = (i < numPoints / 2) ? RED : BLUE;
    points[i] = {x: random(), y: random(), color: color};
  }
  center = computeCenter();
}

function random() {
  return (Math.random() - 0.5) * 10;
}

function computeCenter() {
  let x = 0;
  let y = 0;
  for (let p of points) {
    x += p.x;
    y += p.y;
  }
  return {x: x / points.length, y: y / points.length};
}

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

function rungeKutta(k1) {
  const k2 = k1 + k1 * 0.002 * 0.5;
  const k3 = k1 + k2 * 0.002 * 0.5;
  const k4 = k1 + k3 * 0.002;
  return (k1 + 2 * k2 + 2 * k3 + k4) * (0.002 / 6.0);
}

function distance(p, q) {
  const dx = p.x - q.x;
  const dy = p.y - q.y;
  return Math.sqrt(dx * dx + dy * dy);
}

function computeXV(delta) {
  // Compute values for drawing the graph. First, compute X.
  let sum = 0;
  for (let p of points) {
    sum += distance(center, p);
  }
  let x = points.length / sum;

  // Compute V.
  let newCenter = computeCenter();
  let cx = newCenter.x - center.x;
  let cy = newCenter.y - center.y;
  sum = 0;
  for (let d of delta) {
    let dx = d.x - cx;
    let dy = d.y - cy;
    sum += Math.sqrt(dx * dx + dy * dy);
  }
  let v = sum / points.length;

  center = newCenter;
  xv.push({x: x, v: v});
}

function step() {
  timestep++;
  let ps = [];
  let delta = [];

  for (let pi of points) {
    let x = 0;
    let y = 0;

    for (let pj of points) {
      if (pi == pj) continue;

      const dx = pj.x - pi.x;
      const dy = pj.y - pi.y;
      const dist = Math.sqrt(dx * dx + dy * dy);
      const k = K[pi.color][pj.color];
      x += (k / dist - dist ** -2) * dx / dist;
      y += (k / dist - dist ** -2) * dy / dist;
    }
    x = rungeKutta(x);
    y = rungeKutta(y);
    ps.push({x: pi.x + x, y: pi.y + y, color: pi.color});
    delta.push({x: x, y: y});
  }
  points = ps;

  computeXV(delta);
}

function redrawParams() {
  const p = (K[0][1] + K[1][0]) / 2;

  document.getElementById('k0').value = K[0][0];
  document.getElementById('k1').value = K[0][1];
  document.getElementById('k2').value = K[1][0];
  document.getElementById('k3').value = K[1][1];

  document.getElementById('ra').value = K[0][0];
  document.getElementById('rp').value = p;
  document.getElementById('rm').value = K[0][1] - p;
  document.getElementById('rb').value = K[1][1];
}

function readK() {
  K[0][0] = parseFloat(document.getElementById('k0').value);
  K[0][1] = parseFloat(document.getElementById('k1').value);
  K[1][0] = parseFloat(document.getElementById('k2').value);
  K[1][1] = parseFloat(document.getElementById('k3').value);
  redrawParams();
}

function readR() {
  const a = parseFloat(document.getElementById('ra').value);
  const p = parseFloat(document.getElementById('rp').value);
  const m = parseFloat(document.getElementById('rm').value);
  const b = parseFloat(document.getElementById('rb').value);

  K[0][0] = a;
  K[0][1] = p + m;
  K[1][0] = p - m;
  K[1][1] = b;

  redrawParams();
}

function scaleout() {
  let max = 0;
  for (let p of points) {
    max = Math.max(max, Math.abs(p.x), Math.abs(p.y));
  }

  currentScale = 500 / (max * 1.2);
  ctx.scale(currentScale, currentScale);
}

function redraw() {
  ctx.save();
  ctx.clearRect(-50000, -50000, 100000, 100000);

  drawGraph();
  scaleout();
  drawGrid();

  for (let p of points) {
    drawPoint(p);
  }
  ctx.restore();
  
  document.getElementById('timestep').innerText = timestep;
}

function mainloop() {
  for (let i = 0; i < 500; i++) {
    step();
  }
  redraw();
}

function start() {
  readK();
  handle = window.setInterval(mainloop, 0);
  startButton.innerText = 'Stop';
}

function stop() {
  window.clearInterval(handle);
  handle = undefined;
  startButton.innerText = 'Start';
}

function reset() {
  resetGraph();

  const running = handle;
  if (running) stop();
  initPoints();
  timestep = 0;
  redraw();
  if (running) start();
}

function resetGraph() {
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

function drawGraph() {
  gctx.save();
  gctx.translate(0.25, 0.75);
  gctx.scale(0.25, -0.25);

  gctx.fillStyle = 'red';

  for (let p of xv) {
    let v = Math.log10(1000 * p.v + 1);
    let x = Math.log10(1000 * p.x + 1);

    gctx.beginPath();
    gctx.arc(x, v, 0.01, 0, 2 * Math.PI, true);
    gctx.fill();
  }

  xv = [];
  gctx.restore();
}

startButton.addEventListener('click', function() {
  if (handle) {
    stop();
  } else {
    start();
  }
});

resetButton.addEventListener('click', reset);

for (let elem of document.getElementsByTagName('input')) {
  elem.addEventListener('change', function(event) {
    if (event.currentTarget.id.startsWith('k')) {
      readK();
    } else {
      readR();
    }
    reset();
  });
}

initPoints();
resetGraph();
start();'''


def main(basename):
    write(basename + '.html', gen_html(basename))
    write(basename + '.js', gen_script())


if __name__ == '__main__':
    if len(sys.argv) < 2:
        raise Exception('Arguments must be at least 2.')
    basename = sys.argv[1]
    main(basename)
