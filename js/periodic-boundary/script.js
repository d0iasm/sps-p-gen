// Main canvas
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext('2d');
// The variabel of cycle is defined at c++.
ctx.scale(0.65 / (cycle / 10), 0.65 / (cycle / 10));
ctx.translate(500, 500);

// HTML elements
const startButton = document.getElementById('start');
const resetButton = document.getElementById('reset');
const relationButton = document.getElementById('relation');

// Constants
const RED = 0;
const BLUE = 1;
const BLACK = 2;

const MAX = 1.3;
const MIN = -1.3;

// Variables
let handle;
let index = 0;
let relationOn = true;

function drawGrid() {
  ctx.save();
  ctx.strokeStyle = '#aaa';
  ctx.lineWidth = 0.02;

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
  ctx.translate(-5, -5);
  switch (p.color) {
    case RED:
      ctx.fillStyle = 'red';
      break;
    case BLUE:
      ctx.fillStyle = 'blue';
      break;
    default:
      ctx.fillStyle = 'black';
  }
  ctx.beginPath();
  ctx.arc(p.x, p.y, 0.005 * cycle, 0, 2 * Math.PI, true);
  ctx.fill();

  // Draw Kparam.
  if (relationOn) {
    ctx.lineWidth = 0.01;
    for (let i = 0; i < p.k.length; i++) {
      // Normalize K to -255 to 255 via Y=((X−xmin)/(xmax−xmin)) * (M−m)+m
      // X=K, xmin=MIN (-13), xmax=MAX (-13), M=255, and m=-255.
      const rb = ((p.k[i] - MIN) / (MAX - MIN)) * (255 + 255) - 255;
      ctx.strokeStyle = 'rgb(' + Math.max(0, rb) + ', 0, ' + Math.abs(Math.min(0, rb)) + ')';
      ctx.moveTo(p.x, p.y);
      ctx.lineTo(points[index][i].x, points[index][i].y);
    }
    ctx.stroke();
  }
  ctx.restore();
}

function redraw() {
  ctx.save();
  ctx.clearRect(-50000, -50000, 100000, 100000);
 
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
  handle = window.setInterval(step, 25);
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

function on() {
  relationOn = true;
  relationButton.innerText = 'ON';
  relationButton.style.backgroundColor = "#333";
  relationButton.style.color = "white";
  if (!handle) {
    if (index >= points.length) {
      index = points.length - 1;
    }
    redraw();
  }
}

function off() {
  relationOn = false;
  relationButton.innerText = 'OFF';
  relationButton.style.backgroundColor = "white";
  relationButton.style.color = "#333";
  if (!handle) {
    if (index >= points.length) {
      index = points.length - 1;
    }
    redraw();
  }
}

startButton.addEventListener('click', function() {
  if (handle) {
    stop();
  } else {
    start();
  }
});

resetButton.addEventListener('click', reset);

relationButton.addEventListener('click', function() {
  if (relationOn) {
    off();
  } else {
    on();
  }
});

document.getElementById('timestep').addEventListener('change', e => {
  index = parseInt(e.currentTarget.value / 100);
  redraw();
});

// Fixed scale.
ctx.scale(98, 98);

start();
