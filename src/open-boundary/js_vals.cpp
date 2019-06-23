// Print C++ variables in order that they can be used in JS code.

#include <iostream>
#include "boundary.h"

void importScript() {
  std::cout << "<script src=js/open-boundary/script.js></script>\n";
}

// Do nothing for open boundary.
void printCycle() {
}
