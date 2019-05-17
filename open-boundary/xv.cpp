// This file implements the macroscopic variables to understand the pattern
// of this model. X converges to zero when at least one of the particles
// moves an infinite distance from the center of gravity. V converges to
// zero when the relative velocities of all particles with respect to the
// center of gravity converge to zero.

#include <iostream>
#include <numeric>
#include <vector>
#include "xv.h"

Point computeCenter() {
  double x = 0;
  double y = 0;
  for (Point &p : points) {
    x += p.x;
    y += p.y;
  }
  return {x / NPOINTS, y / NPOINTS};
}

XV computeXV(Point *dxdy) {
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
  Point c = {cx, cy};
  sum = 0;
  for (int i = 0; i < NPOINTS; i++) {
    Point d = dxdy[i];
    sum += distance(c, d);
  }
  double v = sum / NPOINTS;

  center = newCenter;
  return {x, v};
}

static std::vector<std::vector<double> > normalize() {
  double maxx = -1;
  double minx = 1;
  double maxv = -1;
  double minv = 1;
  // Skip first values because an initial position is random.
  for (int i=1; i<xv.size(); i++) {
    if (maxx < xv[i].x) maxx = xv[i].x;
    if (minx > xv[i].x) minx = xv[i].x;
    if (maxv < xv[i].v) maxv = xv[i].v;
    if (minv > xv[i].v) minv = xv[i].v;
  }

  std::vector<std::vector<double> > n(2);
  std::vector<double> nx;
  std::vector<double> nv;
  double dx = maxx - minx;
  double dv = maxv - minv;
  for (int i=1; i<xv.size(); i++) {
    nx.push_back((xv[i].x - minx) / dx);
    nv.push_back((xv[i].v - minv) / dv);
  }
  n[0] = nx;
  n[1] = nv;
  return n;
}

int classify() {
  std::vector<std::vector<double>> n = normalize(); 

  // Calculate center.
  //double sumx = std::accumulate(n[0].begin(), n[0].end(), 0);
  //double sumv = std::accumulate(n[1].begin(), n[1].end(), 0);
  //double gx = sumx / n.size();
  //double gv = sumv / n.size();
  double g = 0.5; 

  // Calculate variance.
  double sumx = 0;
  double sumv = 0;
  for (int i=0; i<n[0].size(); i++) {
    sumx += (n[0][i] - g) * (n[0][i] - g);
    sumv += (n[1][i] - g) * (n[1][i] - g);
  }
  int l = n[0].size();
  double varx = sumx / l;
  double varv = sumv / l;
  
  //std::cerr << "\n------------\n";
  //std::cerr << "start (x, v) " << n[0][0] << ", " << n[1][0] << "\n";
  //std::cerr << "mid   (x, v) " << n[0][l/2] << ", " << n[1][l/2] << "\n";
  //std::cerr << "end   (x, v) " << n[0][l-1] << ", " << n[1][l-1] << "\n";
  //std::cerr << "sumx, sumv: " << sumx << ", " << sumv << "\n"; 
  //std::cerr << "vx, vv: " << varx << ", " << varv << "\n"; 
  //std::cerr << "------------\n";

  if ((varx + varv) / 2 < 0.22) {
    return 5;
  } else if (varx > 0.5 && varv > 0.5) {
    return 6; 
  } else if (n[0][l-1] == 0 && n[1][l-1] == 0) {
    return 1; 
  } else if (n[1][l-1] == 0) {
    return 2;
  } else if (n[0][l-1] == 0) {
    return 3;
  } else if (n[0][l-1] > 0 && n[1][l-1] > 0) {
    return 4;
  }

  return 0; 
}
