#include <omp.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <nlopt.hpp>
#include <sstream>

#include "Cogli1.hpp"
#include "Optimizer.hpp"
#include "ParticleSystem.hpp"
#include "PoreSizeDistribution.hpp"
#include "rng.hpp"

int main() {
  ParticleSystem partSys("configuration.txt");

  PoreSizeDistribution poreDistr(partSys.Lbox);

  std::string cogliFile = "Cogli1/conf.txt";
  Cogli1::box(partSys.Lbox, cogliFile);
  partSys.cogli1(cogliFile, "0,0,1,1", true);

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

/*
double maxRadius(const std::vector<double>& x, std::vector<double>& grad, void* f_data) {
  ParticleSystem* partSys = static_cast<ParticleSystem*>(f_data);

  std::vector<double> center = {x[0], x[1], x[2]};

  double minimumDistance = 1e10;
  for (size_t np = 0; np < partSys->N; np++) {
    double distance = partSys->particles[np].minDistance(center, partSys->Lbox) - partSys->particles[np].R;

    if (distance < minimumDistance) {
      minimumDistance = distance;
    }
  }

  return minimumDistance;
}

double constraintIncludePoint(const std::vector<double>& x, std::vector<double>& grad, void* c_data) {
  ConstraintData* constraintData = static_cast<ConstraintData*>(c_data);

  std::vector<double> center = {x[0], x[1], x[2]};
  double radius = x[3];

  std::array<double, 3> dist;
  double sqrDist = 0.;

  for (int ax = 0; ax < 3; ax++) {
    dist[ax] = center[ax] - constraintData->vecP[ax];
    dist[ax] -= constraintData->Lbox[ax] * rint(dist[ax] / constraintData->Lbox[ax]);
    sqrDist += dist[ax] * dist[ax];
  }

  return sqrt(sqrDist) - radius;
}
*/
