#pragma once

#include <memory>
#include <string>

class SimulationSettings {
 public:
  std::string confFileName;
  std::string dataFolder = "Data";

  // Cogli2 settings
  bool useCogli2 = false;
  std::string cogliFile = "Cogli2/conf.txt";
  std::string sphereColor = "0,0,1,1";
  std::string poreSphereColor = "1,0,0,0.25";
  size_t maxCogliSpheres = 5000;  // Maximum number of spheres to be drawn in Cogli2

  // Loops settings
  double errorTolerance = 1e-2;
  int stepsToAverage = 1000;
  int maxLoops = 100;

  SimulationSettings() = default;

  SimulationSettings(const std::string& settingsFile) {
    loadSettings(settingsFile);
  }

  void loadSettings(const std::string& settingsFile);
};