// Print C++ variables in order that they can be used in JS code.

#include <iostream>
#include "boundary.h"

void importScript() {
  std::cout << "<script src=js/periodic-boundary/script.js></script>\n";
}

void printCycle() {
  std::cout << "<script>\n";
  std::cout << "const cycle = " << cycle << ";\n";
  std::cout << "</script>\n";
}
