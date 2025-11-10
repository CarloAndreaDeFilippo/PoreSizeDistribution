#pragma once

#include "nlohmann/json.hpp"

class SimulationSettings {
 public:
  SimulationSettings() = default;

  SimulationSettings(const std::string& settingsFile) {
    loadSettings(settingsFile);
  }

  void loadSettings(const std::string& settingsFile);
};