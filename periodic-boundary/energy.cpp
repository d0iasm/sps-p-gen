// This file implements calculating the energy based on Heider balance theory.
// The definitions of the energy are (1) average energy of all triangles, and
// (2) variance energy of all triangles. Energy is decided only by K parameters
// and the energy doesn't change during one execution.

#include <iostream>
#include <math.h>
#include "energy.h"
#include "generator.h"

extern double kparam[2][2];

Color getColor(int i) {
  return points[i].color;
}

// Calculate an energy for a triangle based on Heider Balance theory. 
static double heider(int i, int j, int k) {
  int a = getColor(i);
  int b = getColor(j);
  int c = getColor(k);

  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  double p1 = kparam[a][b] * kparam[b][c] * kparam[a][c];
  double p2 = kparam[a][c] * kparam[c][b] * kparam[a][b];
  double p3 = kparam[b][a] * kparam[a][c] * kparam[b][c];
  double p4 = kparam[b][c] * kparam[c][a] * kparam[b][a];
  double p5 = kparam[c][a] * kparam[a][b] * kparam[c][b];
  double p6 = kparam[c][b] * kparam[b][a] * kparam[c][a];

  return (p1 + p2 + p3 + p4 + p5 + p6) / 6;
}

// Calculate an energy for a triangle based on Heider Balance theory.
// This result is influenced from the distance between particles.
static double heider_dist(int i, int j, int k) {
  int a = getColor(i);
  int b = getColor(j);
  int c = getColor(k);

  Point &pi = points[i];
  Point &pj = points[j];
  Point &pk = points[k];

  // Distance between a and b.
  double dx_ij = pj.x - pi.x;
  double dy_ij = pj.y - pi.y;
  double dist_ij = sqrt(dx_ij * dx_ij + dy_ij * dy_ij);

  // Distance between a and c.
  double dx_ik = pk.x - pi.x;
  double dy_ik = pk.y - pi.y;
  double dist_ik = sqrt(dx_ik * dx_ik + dy_ik * dy_ik);

  // Distance between b and c.
  double dx_jk = pj.x - pk.x;
  double dy_jk = pj.y - pk.y;
  double dist_jk = sqrt(dx_jk * dx_jk + dy_jk * dy_jk);
  
  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  double p1 = (kparam[a][b] / dist_ij)
            * (kparam[b][c] / dist_jk)
            * (kparam[a][c] / dist_ik);
  double p2 = (kparam[a][c] / dist_ik)
            * (kparam[c][b] / dist_jk)
            * (kparam[a][b] / dist_ij);
  double p3 = (kparam[b][a] / dist_ij)
            * (kparam[a][c] / dist_ik)
            * (kparam[b][c] / dist_jk);
  double p4 = (kparam[b][c] / dist_jk)
            * (kparam[c][a] / dist_ik)
            * (kparam[b][a] / dist_ij);
  double p5 = (kparam[c][a] / dist_ik)
            * (kparam[a][b] / dist_ij)
            * (kparam[c][b] / dist_jk);
  double p6 = (kparam[c][b] / dist_jk)
            * (kparam[b][a] / dist_ij)
            * (kparam[c][a] / dist_ik);
 
  //std::cerr << heider(i, j, k) << ", "
  //  << (p1 + p2 + p3 + p4 + p5 + p6) / 6 << ", "
  //  << dist_ij << ", " 
  //  << dist_ik << ", " 
  //  << dist_jk << "\n";
  return (p1 + p2 + p3 + p4 + p5 + p6) / 6;
}

// Calculate an average energy of all triangles.
double energy_ave() {
  int size = 0;
  double sum = 0; 
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        sum += heider(i, j, k);
      }
    }
  }
  return sum / size;
}

// Calculate a variance energy of all triangles.
double energy_var() {
  double average = 0;
  average += kparam[0][0];
  average += kparam[0][1];
  average += kparam[1][0];
  average += kparam[1][1];
  average /= 4;
 
  int size = 0;
  double sum = 0;
  double tmp = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        tmp = heider(i, j, k);
        sum += (tmp - average) * (tmp - average);
      }
    }
  }
  return sum / size;
}

// Calculate a variance energy of all triangles. This result is influenced
// from distance between particles.
double energy_var_dist() {
  double average = 0;
  average += kparam[0][0];
  average += kparam[0][1];
  average += kparam[1][0];
  average += kparam[1][1];
  average /= 4;
 
  int size = 0;
  double sum = 0;
  double tmp = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        tmp = heider_dist(i, j, k);
        sum += (tmp - average) * (tmp - average);
      }
    }
  }
  return sum / size;
}

