#pragma once

#include <string>

#include "SimulationSettings.hpp"

class PoreDistributionSimulation {
 public:
  SimulationSettings simSettings;

  PoreDistributionSimulation() = default;

  PoreDistributionSimulation(const std::string& simFile) : simSettings(simFile) {};

  void Simulation();
};