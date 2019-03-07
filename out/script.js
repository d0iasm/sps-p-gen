// Main canvas
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext('2d');
ctx.scale(0.65, 0.65);
ctx.translate(500, 500);

// Log log graph
const graph = document.getElementById("graph");
const gctx = graph.getContext('2d');
gctx.scale(250, 250);

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
  gctx.save();
  gctx.clearRect(0, 0, 1, 1);
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

  document.getElementById('timestep').value = points[index][0];
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
  drawGraphGrid();
  drawGraph();

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

document.getElementById('timestep').addEventListener('change', e => {
  index = parseInt(e.currentTarget.value / 100);
  drawGraphGrid();
  drawGraph();
  redraw();
});

drawGraphGrid();
drawGraph();
start();
