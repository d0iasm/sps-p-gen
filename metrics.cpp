/**
 * @return Balance value by all triangles. 
 */
double balance() {
  int size = 0;
  double sum = 0; 
  for (int i = 0; i < NPOINTS-2; i++) {
    for (int j = i+1; j < NPOINTS-1; j++) {
      for (int k = j+1; k < NPOINTS; k++) {
        size++;
        sum += average(i, j, k);
      }
    }
  }
  return sum / size;
}

/**
 * Calculate the balance value per one triangle in the average way. 
 * i.e. c[] = {1, 4, 39};
 * {1, 4}, {1, 39}, {4, 39}
 * Calculate the average of each combination.
 * (K(1->4) + K(4->1)) / 2
 * (K(1->39) + K(39->1)) / 2
 * (K(4->39) + K(39->3)) / 2
 * Then, multiply all averages.
 *
 * @param pi First point. 
 * @param pj Second point. 
 * @param pk Third point. 
 * @return Balance value per one triangle. 
 */
double average(int pi, int pj, int pk) {
  int pcol = getColor(getColor[pi]);
  int ocol = getColor(getColor[pj]);
  int xcol = getColor(getColor[pk]);

  double a = (kparam[pcol][ocol] + kparam[ocol][pcol]) / 2;
  double b = (kparam[pcol][xcol] + kparam[xcol][pcol]) / 2;
  double c = (kparam[ocol][xcol] + kparam[xcol][ocol]) / 2;

  return a * b * c;
}

/**
 * Calculate the balance value per one triangle in the Heider theory.
 * In HB theory, only P->O, P->X, O->X directions are valid.
 * i.e. c[] = {1, 4, 39};
 * P  |  O  |  X
 * ===============
 * 1  |  4  |  39
 * 1  |  39 |  4
 * 4  |  1  |  39
 * 4  |  39 |  1
 * 39 |  1  |  4
 * 39 |  4  |  1
 * Calculate the average of the sum of each pattern.
 * ( K(1->4) * K(1->39) * K(4->39)
 * + K(1->39) * K(1->4) * K(4->39)
 * + K(4->1) * K(4->39) * K(1->39)
 * + ....
 * ) / 6
 *
 * @param pi First point. 
 * @param pj Second point. 
 * @param pk Third point. 
 * @return Balance value per one triangle. 
 */
double heider(int pi, int pj, int pk) {
  int a = getColor(getColor[pi]);
  int b = getColor(getColor[pj]);
  int c = getColor(getColor[pk]);

  // p->o, o->x, p->x
  double p1 = kparam[a][b] * kparam[b][c] * kparam[a][c];
  double p2 = kparam[a][c] * kparam[c][b] * kparam[a][b];
  double p3 = kparam[b][a] * kparam[a][c] * kparam[b][c];
  double p4 = kparam[b][c] * kparam[c][a] * kparam[b][a];
  double p5 = kparam[c][a] * kparam[a][b] * kparam[c][b];
  double p6 = kparam[c][b] * kparam[b][a] * kparam[c][a];

  return (p1 * p2 * p3 * p4 * p5 * p6) / 6;
}

