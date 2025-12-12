#include "PoreDistributionSimulation.hpp"

#include <omp.h>

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <nlopt.hpp>
#include <sstream>

#include "Cogli2.hpp"
#include "LinkedCellList.hpp"
#include "Optimizer.hpp"
#include "ParticleSystem.hpp"
#include "PoreSizeDistribution.hpp"
#include "Utilities.hpp"
#include "rng.hpp"

void PoreDistributionSimulation::Simulation() {
  ParticleSystem partSys(simSettings.confFileName);

  PoreSizeDistribution poreDistr(partSys.Lbox);
  poreDistr.errorTolerance = simSettings.errorTolerance;

  if (!directoryExists(simSettings.dataFolder)) makeDirectory(simSettings.dataFolder);

  if (simSettings.useCogli2) {
    if (!directoryExists("Cogli2")) makeDirectory("Cogli2");
    Cogli2::box(partSys.Lbox, simSettings.cogliFile);
    partSys.cogli2(simSettings.cogliFile, simSettings.sphereColor, true);
  }

  // Setup LinkedCellList for random point
  LinkedCellList<Sphere> cellList(partSys.Lbox);

  double maxParticleDistance = 0.;

  for (const auto& np : partSys.particles) maxParticleDistance = std::max(maxParticleDistance, np.D);

  cellList.setCellMinWidth(maxParticleDistance);
  cellList.createList(partSys.particles);

  int loopNumber = 0;

  do {
    std::cout << "Loop #" << loopNumber << "\n";
    loopNumber++;

    poreDistr.oldPoreSizeDistr = poreDistr.poreSizeDistr;

#ifdef USE_OPENMP
#pragma omp parallel for
#endif
    for (int step = 0; step < simSettings.stepsToAverage; step++) {
      Optimizer optimizer(partSys.Lbox);

      optimizer.randomPoint(partSys, cellList);
      optimizer.initializeOptimization(partSys);

      optimizer.optimize();

      if (simSettings.useCogli2 && poreDistr.poreSpheres.size() < simSettings.maxCogliSpheres) {
        Sphere sph(2. * optimizer.maxDist);
        sph.tf.cm = {optimizer.x[0], optimizer.x[1], optimizer.x[2]};
        poreDistr.insertSphere(sph);
      }

      poreDistr.insertData(2. * optimizer.maxDist);
    }

    if (loopNumber > 1) {
      poreDistr.computeAverageError(loopNumber, simSettings.stepsToAverage);
      std::cout << "Average error: " << poreDistr.averageError << "\n";
    }

  } while (poreDistr.averageError > poreDistr.errorTolerance && loopNumber < simSettings.maxLoops);

  if (simSettings.useCogli2)
    poreDistr.cogliPore(simSettings.cogliFile, simSettings.poreSphereColor, true);

  poreDistr.normalizeHistogram((loopNumber + 1.) * simSettings.stepsToAverage);
  poreDistr.saveHistogram(simSettings.dataFolder + "/Histogram.txt");
}