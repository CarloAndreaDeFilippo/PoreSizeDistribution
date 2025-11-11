#include "SimulationSettings.hpp"

#include <fstream>
#include <iostream>

#include "nlohmann/json.hpp"

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
    std::cout << it.key() << ": " << it.value() << "\n";
  }

  // confFileName

  if (!settings.contains("confFileName")) {
    std::cout << "Error! Missing configuration file name confFileName\n";
    exit(-1);
  }
  confFileName = settings["confFileName"];

  // dataFolder name
  if (settings.contains("dataFolder"))
    dataFolder = settings["dataFolder"];

  // cogli2 settings
  if (settings.contains("useCogli2") && settings["useCogli2"].get<bool>() == true) {
    useCogli2 = true;

    if (settings.contains("cogliFile")) cogliFile = settings["cogliFile"];

    if (settings.contains("sphereColor")) sphereColor = settings["sphereColor"];

    if (settings.contains("poreSphereColor")) poreSphereColor = settings["poreSphereColor"];

    if (settings.contains("maxCogliSpheres")) maxCogliSpheres = settings["maxCogliSpheres"].get<size_t>();
  }

  // Loop settings
  if (settings.contains("stepsToAverage")) stepsToAverage = settings["stepsToAverage"].get<int>();

  if (settings.contains("maxLoops")) maxLoops = settings["maxLoops"].get<int>();
}