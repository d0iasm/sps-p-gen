// Main canvas.
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext('2d');
ctx.scale(0.65, 0.65);
ctx.translate(500, 500);

// HTML elements.
const startButton = document.getElementById('start');
const resetButton = document.getElementById('reset');
const relationButton = document.getElementById('relation');

// Constants.
const RED = 0;
const BLUE = 1;
const BLACK = 2;

const MAX = 1.3;
const MIN = -1.3;

// Variables.
let currentScale = 1;
let handle;
let index = 0;
let relationOn = true;

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
  ctx.arc(p.x, p.y, 5 / currentScale, 0, 2 * Math.PI, true);
  ctx.fill();

  // Draw Kparam.
  if (relationOn) {
    ctx.lineWidth = 0.05;
    for (let i = 0; i < p.k.length; i++) {
      // Normalize K to -255 to 255 via Y=((X−xmin)/(xmax−xmin)) * (M−m)+m
      // X=K, xmin=MIN (-13), xmax=MAX (-13), M=255, and m=-255.
      const rb = ((p.k[i] - MIN) / (MAX - MIN)) * (255 + 255) - 255;
      ctx.strokeStyle = 'rgba(' + Math.max(0, rb) + ', 0, ' + Math.abs(Math.min(0, rb)) + ', 0.4)';
      ctx.moveTo(p.x, p.y);
      ctx.lineTo(points[index][i].x, points[index][i].y);
    }
    ctx.stroke();
    }
  ctx.restore();
}

function drawKparam(kparam) {
  ctx.save();
  ctx.lineWidth = 0.01;
  for (let i = 1; i < points[index].length; i++) {
    const k = kparam[0][i-1]; // Kparam of 0 to an other particle.
    // Normalize to [0..255];
    // TODO: positive relashionship represents red and negative one do blue.
    // The value around 0 shows around white.
    const r = ((k - MIN) / (MAX - MIN)) * (255 - 0) + 0;
    ctx.strokeStyle = 'rgb(' + r + ', 0, 0)';
    ctx.moveTo(points[index][i].x, points[index][i].y);
    ctx.lineTo(points[index][1].x, points[index][1].y);
  }
  ctx.stroke();
  /*
  ctx.restore();
  ctx.save();
  ctx.lineWidth = 0.01;
  for (let i = 0; i < kparam.length; i++) {
    for (let j = 0; j < kparam[0].length; j++) {
      ctx.fillStyle = 'rgb(255, 0, 0)';
      ctx.moveTo(points[index][i].x, points[index][i].y);
      ctx.lineTo(points[index][j].x, points[index][j].y)
      ctx.stroke();
    }
  }
  */
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

function redraw() {
  ctx.save();
  ctx.clearRect(-50000, -50000, 100000, 100000);
 
  scaleout();
  drawGrid();

  // i starts 1 because points[index][0] represents the number of steps.
  for (let i = 1; i < points[index].length; i++) {
    drawPoint(points[index][i]);
  }

  // kparam[index].step represents the number of steps.
  // kparam[index].k represents 2d array kparams.
  //for (let i = 0; i < kparam[index].k.length; i++) {
    //drawKparam(kparam[index].k);
  //}

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
  handle = window.setInterval(step, 50);
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

start();
