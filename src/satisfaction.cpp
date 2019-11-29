// Calculate the satisfaction for a particle based on triangles which is composed of the target particle and other paritcles.  

#include "generator.h"
#include "energy.h"
#include <iostream>

// Calcualte the satisfaction of a target particle. 
// The number of triangles is 2352 (=49P2).
// The minimum satisfaction is 0 and the maximum is 2352 (= 1.0*2352).
double satisfaction(int p) {
  double sum = 0;
  for (int o=0; o < NPOINTS; o++) {
    if (p == o)
      continue;
    for (int x=0; x < NPOINTS; x++) {
      if (x == p || x == o)
        continue;

      sum += heiderSpaceLocality(p, o, x);
    }
	}
  return sum;
}
