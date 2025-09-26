#include <omp.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlopt.hpp>
#include <sstream>

#include "Cogli2.hpp"
#include "Optimizer.hpp"
#include "ParticleSystem.hpp"
#include "PoreSizeDistribution.hpp"
#include "Utilities.hpp"
#include "rng.hpp"

int main() {
  ParticleSystem partSys("configuration.txt");

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
  int maxLoops = 1000;

  do {
    std::cout << "Loop #" << loopNumber << "\n";
    loopNumber++;

    poreDistr.oldPoreSizeDistr = poreDistr.poreSizeDistr;

#pragma omp parallel for
    for (int step = 0; step < stepsToAverage; step++) {
      Optimizer optimizer(partSys.Lbox);

      optimizer.randomPoint(partSys);
      optimizer.initializeOptimization(partSys);

      optimizer.optimize();

      if (poreDistr.poreSpheres.size() < maxCogliSpheres) {
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

  poreDistr.cogliPore(cogliFile, "1,0,0,0.25", true);

  poreDistr.normalizeHistogram((loopNumber + 1.) * stepsToAverage);
  poreDistr.saveHistogram(dataFolder + "/Histogram.txt");

  return 0;
}
