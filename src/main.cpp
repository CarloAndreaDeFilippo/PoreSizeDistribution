#include <omp.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlopt.hpp>
#include <sstream>

#include "Cogli2.hpp"
#include "LinkedCellList.hpp"
#include "Optimizer.hpp"
#include "ParticleSystem.hpp"
#include "PoreSizeDistribution.hpp"
#include "Utilities.hpp"
#include "rng.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Error, specify the name of the configuration file.\n";
    exit(-1);
  }

  std::string confFileName = argv[1];

  ParticleSystem partSys(confFileName);

  PoreSizeDistribution poreDistr(partSys.Lbox);

  std::string dataFolder = "Data";
  if (!directoryExists(dataFolder)) makeDirectory(dataFolder);

  bool useCogli2 = true;
  std::string cogliFile = "Cogli2/conf.txt";

  if (useCogli2) {
    if (!directoryExists("Cogli2")) makeDirectory("Cogli2");
    Cogli2::box(partSys.Lbox, cogliFile);
    partSys.cogli2(cogliFile, "0,0,1,1", true);
  }

  size_t maxCogliSpheres = 5000;  // Maximum number of spheres to be drawn in Cogli2
  int stepsToAverage = 1000;
  int loopNumber = 0;
  int maxLoops = 100;

  // Setup LinkedCellList for random point
  LinkedCellList<Sphere> cellList;

  double maxParticleDistance = 0.;

  for (const auto& np : partSys.particles) maxParticleDistance = std::max(maxParticleDistance, np.D);

  cellList.setCellMinWidth(maxParticleDistance);
  cellList.createList(partSys.particles, partSys.Lbox);

  do {
    std::cout << "Loop #" << loopNumber << "\n";
    loopNumber++;

    poreDistr.oldPoreSizeDistr = poreDistr.poreSizeDistr;

#pragma omp parallel for
    for (int step = 0; step < stepsToAverage; step++) {
      Optimizer optimizer(partSys.Lbox);

      optimizer.randomPoint(partSys, cellList);
      optimizer.initializeOptimization(partSys);

      optimizer.optimize();

      if (useCogli2 && poreDistr.poreSpheres.size() < maxCogliSpheres) {
        Sphere sph(2. * optimizer.maxDist);
        sph.tf.cm = {optimizer.x[0], optimizer.x[1], optimizer.x[2]};
        poreDistr.insertSphere(sph);
      }

      poreDistr.insertData(2. * optimizer.maxDist);
    }

    if (loopNumber > 1) {
      poreDistr.computeAverageError(loopNumber, stepsToAverage);
      std::cout << "Average error: " << poreDistr.averageError << "\n";
    }

  } while (poreDistr.averageError > poreDistr.errorTolerance && loopNumber < maxLoops);

  if (useCogli2)
    poreDistr.cogliPore(cogliFile, "1,0,0,0.25", true);

  poreDistr.normalizeHistogram((loopNumber + 1.) * stepsToAverage);
  poreDistr.saveHistogram(dataFolder + "/Histogram.txt");

  return 0;
}
