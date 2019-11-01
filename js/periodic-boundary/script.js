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

const MAX = 1.2;
const MIN = -1.2;

// Variables
let handle;
let timestep = 0;
let relationOn = true;

function drawGrid() {
  ctx.save();
  ctx.strokeStyle = '#aaa';
  ctx.lineWidth = 0.002 * cycle;

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

// Fix the p1's position and move p2 to 9 virtual positions.
// Get the positon of the closest virtual image of p2.
function closest(p1, p2) {
  let closest = Number.MAX_SAFE_INTEGER;
  const x1 = p1.x;
  const y1 = p1.y;
  const x2 = p2.x;
  const y2 = p2.y;

  let closestDistance = Number.MAX_SAFE_INTEGER;
  let closestX = x2;
  let closestY = y2;

  for (let i=-1; i<=1; i++) {
    for (let j=-1; j<=1; j++) {
      const dx = (x2 + cycle * i) - x1;
      const dy = (y2 + cycle * j) - y1;
      const tmp = Math.sqrt(dx * dx + dy * dy);
      if (tmp < closestDistance) {
        closestDistance = tmp;
        closestX = x2 + cycle * i;
        closestY = y2 + cycle * j;
      }
    }
  }
  return { "x": closestX, "y": closestY };
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
    ctx.lineWidth = 0.004 * cycle;
    // Index i is the target point.
    for (let i = 0; i < p.k.length; i++) {
      // Normalize K to -255 to 255 via Y=((X−xmin)/(xmax−xmin)) * (M−m)+m
      // X=K, xmin=MIN (-13), xmax=MAX (-13), M=255, and m=-255.
      const rb = ((p.k[i] - MIN) / (MAX - MIN)) * (255 + 255) - 255;
      ctx.strokeStyle = 'rgba(' + Math.max(0, rb) + ', 0, ' + Math.abs(Math.min(0, rb)) + ', 0.4)';

      // Find the closest virtual position.
      q = closest(p, points[timestep][i+1]);

      ctx.beginPath();
      ctx.moveTo(p.x, p.y);
      // Note: the target starts from index 1.
      ctx.lineTo(q.x, q.y);
      ctx.stroke();
    }
  }
  ctx.restore();
}

function redraw() {
  ctx.save();
  ctx.clearRect(-50000, -50000, 100000, 100000);

  drawGrid();

  for (let i = 1; i < points[timestep].length; i++) {
    drawPoint(points[timestep][i]);
  }

  document.getElementById('timestep').value = points[timestep][0];
  ctx.restore();
}

function step() {
  if (timestep < points.length) {
    redraw();
    timestep++;
  } else {
    stop();
  }
}

function start() {
  handle = window.setInterval(step, 10);
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
  timestep = 0;
  redraw();
  if (running) start();
}

function on() {
  relationOn = true;
  relationButton.innerText = 'ON';
  relationButton.style.backgroundColor = "#333";
  relationButton.style.color = "white";
  if (!handle) {
    if (timestep >= points.length) {
      timestep = points.length - 1;
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
    if (timestep >= points.length) {
      timestep = points.length - 1;
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
  timestep = parseInt(e.currentTarget.value / 100);
  redraw();
});

// Fixed scale.
ctx.scale(98, 98);

start();
