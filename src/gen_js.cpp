// Generate javascript based on data.

#include <cstring>
#include <iostream>
#include "generator.h"
#include "steps.pb.h"

void js(data::Steps steps) {
  std::ofstream js;
  js.open("js/data/" + filename() + ".js");

  js << "const steps = [";
  for (int step=0; step<steps.steps_size(); step++) {
    js << "\n{"
       << "step:" << steps.steps()[step].step() << ","
       << "particles: [";
    for (int i=0; i<NPOINTS; i++) {
      auto p = steps.steps()[step].particles()[i];
      js << "{"
         << "x:" << p.x() << ","
         << "y:" << p.y() << ","
         << "k:[";
      // kparams are int because it's a discrete value
      auto k = p.kparams();
      for (int j=0; j<NPOINTS; j++) {
        js << k[j] / 10.0 << ",";
      }
      js << "]},";
    }
    js << "]},";
  }
  js << "]";
}
