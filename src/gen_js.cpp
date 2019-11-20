// Generate javascript based on data.

#include <cstring>
#include <iostream>
#include "generator.h"
#include "steps.pb.h"

void js(data::Steps steps) {
  std::ofstream js;
  js.open("js/data/" + filename() + ".js");

  js << "const steps = [\n";
  for (int step=0; step<steps.steps_size(); step++) {
    js << "{\n"
       << "step:" << steps.steps()[step].step() << ","
       << "particles: [\n";
    for (int i=0; i<NPOINTS; i++) {
      auto p = steps.steps()[step].particles()[i];
      js << "{"
         << "x:" << p.x() << ","
         << "y:" << p.y() << ","
         << "k:[";
      for (int j=0; j<NPOINTS; j++) {
        auto k = p.kparams()[j];
        js << k.val() << ",";
      }
      js << "]},";
    }
    js << "]},";
  }
  js << "]";
}
