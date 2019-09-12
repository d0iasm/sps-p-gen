// This file implements calculating the energy based on Heider balance theory.

#include <iostream>
#include <math.h>
#include "boundary.h"
#include "generator.h"
#include "energy.h"

// Calculate an energy for a triangle using Heider Balance theory.
// The triangle has directed, p->o, o->x, and p->x, edges.
// Note: Energy takes a negative value!
static double heider(int p, int o, int x) {
  return - (kparam[p][o] * kparam[o][x] * kparam[p][x]);
}

// Calculate an energy for a triangle based on Heider Balance theory.
// The triangle has undirected edges, so take an average of all patterns.
static double heiderUndirected(int i, int j, int k) {
  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  double p1 = heider(i, j, k);
  double p2 = heider(i, k, j);
  double p3 = heider(j, i, k);
  double p4 = heider(j, k, i);
  double p5 = heider(k, i, j);
  double p6 = heider(k, j, i);
  return (p1 + p2 + p3 + p4 + p5 + p6) / 6;
}

// Calculate an energy for a triangle based on Heider Balance theory.
// K parameters are influenced from the distance between particles.
static double heiderUndirectedSpaceLocality(int i, int j, int k) {
  Point &pi = points[i];
  Point &pj = points[j];
  Point &pk = points[k];

  // Distance between a and b.
  double dist_ij = distance(pi, pj);

  // Distance between a and c.
  double dist_ik = distance(pi, pk);

  // Distance between b and c.
  double dist_jk = distance(pj, pk);

  double dist = dist_ij * dist_ik * dist_jk;

  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  // k(i->j)/distance(i,j) * k(i->k)/distance(i,k) * k(j->k)/distance(j,k)
  double p1 = heider(i, j, k) / dist;
  double p2 = heider(i, k, j) / dist;
  double p3 = heider(j, i, k) / dist;
  double p4 = heider(j, k, i) / dist;
  double p5 = heider(k, i, j) / dist;
  double p6 = heider(k, j, i) / dist;
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
        sum += heiderUndirected(i, j, k);
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
        tmp = heiderUndirected(i, j, k);
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
        sum += heiderUndirectedSpaceLocality(i, j, k);
      }
    }
  }
  return - (sum / size);
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
        tmp = heiderUndirectedSpaceLocality(i, j, k);
        sum += (tmp - average) * (tmp - average);
      }
    }
  }
  return sum / size;
}

// Calculate a local energy that could affect the relationship
// between P and O particles.
double energyLocal(int p, int o) {
  double sum = 0;
  for (int x=0; x < NPOINTS; x++) {
    if (x == p || x == o)
      continue;

    sum += heider(p, o, x);
	}
  return sum;
}
