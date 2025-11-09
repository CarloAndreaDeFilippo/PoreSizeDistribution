#include "SimulationSettings.hpp"

#include <fstream>
#include <iostream>

void SimulationSettings::loadSettings(const std::string& settingsFile) {
  std::ifstream file(settingsFile);
  if (!file.is_open()) {
    std::cout << "Error opening " << settingsFile << "\n";
    std::exit(-1);
  }

  // Read simulation parameters from json file
  nlohmann::json settings;
  file >> settings;
  file.close();

  // Print chosen settings
  for (auto it = settings.begin(); it != settings.end(); ++it) {
    std::cout << it.key() << ": " << it.value() << '\n';
  }
}