#include "energy.h"
#include "generator.h"

extern double kparam[2][2];

Color getColor(int i) {
  return points[i].color;
}

// Calculate an energy for a triangle based on Heider Balance theory. 
double heider(int pi, int pj, int pk) {
  int a = getColor(pi);
  int b = getColor(pj);
  int c = getColor(pk);

  // Permutation(3) = 6 patterns p->o, o->x, p->x.
  double p1 = kparam[a][b] * kparam[b][c] * kparam[a][c];
  double p2 = kparam[a][c] * kparam[c][b] * kparam[a][b];
  double p3 = kparam[b][a] * kparam[a][c] * kparam[b][c];
  double p4 = kparam[b][c] * kparam[c][a] * kparam[b][a];
  double p5 = kparam[c][a] * kparam[a][b] * kparam[c][b];
  double p6 = kparam[c][b] * kparam[b][a] * kparam[c][a];

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

