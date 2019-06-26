// This file implements calculating the energy based on Heider balance theory.

#include <iostream>
#include <math.h>
#include "boundary.h"
#include "generator.h"
#include "energy.h"

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
// K parameters are influenced from the distance between particles.
static double heiderDist(int i, int j, int k) {
  int a = getColor(i);
  int b = getColor(j);
  int c = getColor(k);

  Point &pi = points[i];
  Point &pj = points[j];
  Point &pk = points[k];

  // Distance between a and b.
  double dist_ij = distance(pi, pj);

  // Distance between a and c.
  double dist_ik = distance(pi, pk);

  // Distance between b and c.
  double dist_jk = distance(pj, pk);
 
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
 
  return (p1 + p2 + p3 + p4 + p5 + p6) / 6;
}

// Calculate an average energy of all triangles.
double energyAverage() {
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
double energyVariance() {
  double average = energyAverage();
  int size = 0;
  double sum = 0;
  double tmp = 0;
  for (int i = 0; i < NPOINTS - 2; i++) {
    for (int j = i+1; j < NPOINTS - 1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        tmp = heider(i, j, k);
        sum += (tmp - average) * (tmp - average);
      }
    }
  }
  return sum / size;
}

// Calculate an average energy of all triangles for each step.
// K parameters are influenced from distance between particles.
double energyAverageDist() {
  int size = 0;
  double sum = 0; 
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        sum += heiderDist(i, j, k);
      }
    }
  }
  return sum / size;
}

// Calculate a variance energy of all triangles for each step.
// K parameters are influenced from distance between particles.
double energyVarianceDist() {
  double average = energyAverage();
  int size = 0;
  double sum = 0;
  double tmp = 0;
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        tmp = heiderDist(i, j, k);
        sum += (tmp - average) * (tmp - average);
      }
    }
  }
  return sum / size;
}

