// This file implements calculating the energy based on Heider balance theory.

#include <iostream>
#include <math.h>
#include "boundary.h"
#include "generator.h"
#include "energy.h"

// Calculate an energy for a triangle based on Heider Balance theory. 
static double heider(int i, int j, int k) {
  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  double p1 = kparam[i][j] * kparam[j][k] * kparam[i][k];
  double p2 = kparam[i][k] * kparam[k][j] * kparam[i][j];
  double p3 = kparam[j][i] * kparam[i][k] * kparam[j][k];
  double p4 = kparam[j][k] * kparam[k][i] * kparam[j][i];
  double p5 = kparam[k][i] * kparam[i][j] * kparam[k][j];
  double p6 = kparam[k][j] * kparam[j][i] * kparam[k][i];

  return (p1 + p2 + p3 + p4 + p5 + p6) / 6;
}

// Calculate an energy for a triangle based on Heider Balance theory.
// K parameters are influenced from the distance between particles.
static double heiderDist(int i, int j, int k) {
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
  double p1 = (kparam[i][j] / dist_ij)
            * (kparam[j][k] / dist_jk)
            * (kparam[i][k] / dist_ik);
  double p2 = (kparam[i][k] / dist_ik)
            * (kparam[k][j] / dist_jk)
            * (kparam[i][j] / dist_ij);
  double p3 = (kparam[j][i] / dist_ij)
            * (kparam[i][k] / dist_ik)
            * (kparam[j][k] / dist_jk);
  double p4 = (kparam[j][k] / dist_jk)
            * (kparam[k][i] / dist_ik)
            * (kparam[j][i] / dist_ij);
  double p5 = (kparam[k][i] / dist_ik)
            * (kparam[i][j] / dist_ij)
            * (kparam[k][j] / dist_jk);
  double p6 = (kparam[k][j] / dist_jk)
            * (kparam[j][i] / dist_ij)
            * (kparam[k][i] / dist_ik);
 
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

