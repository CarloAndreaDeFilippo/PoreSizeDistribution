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
#include "rng.hpp"

int main() {
  ParticleSystem partSys("configuration.txt");

  PoreSizeDistribution poreDistr(partSys.Lbox);

  std::string cogliFile = "Cogli2/conf.txt";
  Cogli2::box(partSys.Lbox, cogliFile);
  partSys.cogli2(cogliFile, "0,0,1,1", true);

  size_t maxCogliSpheres = 5000;
  size_t totSteps = 1000;

  // #pragma omp parallel for
  for (size_t step = 0; step < totSteps; step++) {
    std::cout << "Step " << step << "\n";

    Optimizer optimizer(partSys.Lbox);

    optimizer.randomP(partSys);
    optimizer.initializeOptimization(partSys);

    optimizer.optimize();

    if (poreDistr.poreSpheres.size() < maxCogliSpheres) {
      Sphere sph(2. * optimizer.maxDist);
      sph.tf.cm = {optimizer.x[0], optimizer.x[1], optimizer.x[2]};
      poreDistr.insertSphere(sph);
    }

    poreDistr.insertData(2. * optimizer.maxDist);
  }

  poreDistr.normalizeHistogram(totSteps);

  poreDistr.cogliPore(cogliFile, "1,0,0,0.25", true);

  poreDistr.saveHistogram("Histogram.txt");

  return 0;
}
