#include "Optimizer.hpp"

#include "rng.hpp"

void Optimizer::randomPoint(const ParticleSystem& partSys, const LinkedCellList<Sphere>& cellList) {
  bool overlap = true;
  while (overlap == true) {
    overlap = false;

    for (int ax = 0; ax < 3; ax++)
      constrData.vecP[ax] = rng.randomDouble(-0.5 * partSys.Lbox[ax], 0.5 * partSys.Lbox[ax]);

    // TODO: Placeholder for systems with spheres of different sizes
    double overlapDistance = 2. * partSys.particles[0].D;

    if (cellList.minDistance(constrData.vecP, partSys.particles, partSys.Lbox) < overlapDistance) overlap = true;
  }
}

double maxRadius(const std::vector<double>& x, std::vector<double>& grad, void* f_data) {
  ParticleSystem* partSys = static_cast<ParticleSystem*>(f_data);

  std::array<double, 3> center = {x[0], x[1], x[2]};

  double minimumDistance = 1e10;
  // TODO: make a function that finds the closest sphere (even if they are not in the 27 surrounding cells)
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

void Optimizer::initializeOptimization(ParticleSystem& partSys) {
  x = {constrData.vecP[0], constrData.vecP[1], constrData.vecP[2], 2.0};

  opt.set_max_objective(maxRadius, &partSys);
  opt.add_inequality_constraint(constraintIncludePoint, &constrData, 1e-6);
  opt.set_xtol_rel(1e-4);  // Tolerance for stopping
}

void Optimizer::optimize() {
  opt.optimize(x, maxDist);
}
