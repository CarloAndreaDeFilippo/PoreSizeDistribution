
#include <iostream>

#include "PoreDistributionSimulation.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Error, specify the name of the configuration file.\n";
    exit(-1);
  }

  std::string confFileName = argv[1];

  PoreDistributionSimulation poreDistrSim(argv[1]);

  poreDistrSim.Simulation();

  return 0;
}
