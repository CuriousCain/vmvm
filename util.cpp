#include "util.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>

void util::dump(std::vector<int> data, std::string name) {
  std::cout << name << " DUMP\n";
  for(auto& i:data) {
    std::cout << i << std::endl;
  }
};
