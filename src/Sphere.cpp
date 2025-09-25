#include "Sphere.hpp"

#include <cmath>

double Sphere::minDistance(const Sphere& np, const std::array<double, 3>& Lbox) {
  std::array<double, 3> dist;
  double sqrDist = 0.;

  for (int ax = 0; ax < 3; ax++) {
    dist[ax] = tf.cm[ax] - np.tf.cm[ax];
    dist[ax] -= Lbox[ax] * rint(dist[ax] / Lbox[ax]);
    sqrDist += dist[ax] * dist[ax];
  }

  return sqrt(sqrDist);
}

bool Sphere::overlapCheck(const Sphere& np, const std::array<double, 3>& Lbox) {
  double averageDiameter = R + np.R;

  if (minDistance(np, Lbox) <= averageDiameter) return true;

  return false;
}

double Sphere::minDistance(const std::vector<double> cm, const std::array<double, 3>& Lbox) {
  std::array<double, 3> dist;
  double sqrDist = 0.;

  for (int ax = 0; ax < 3; ax++) {
    dist[ax] = tf.cm[ax] - cm[ax];
    dist[ax] -= Lbox[ax] * rint(dist[ax] / Lbox[ax]);
    sqrDist += dist[ax] * dist[ax];
  }

  return sqrt(sqrDist);
}

std::string Sphere::cogli2(std::string color, const std::array<double, 3>& Lbox) {
  std::string output = "";

  for (int ax = 0; ax < 3; ax++) {
    output += std::to_string(tf.cm[ax] + 0.5 * Lbox[ax]) + " ";
  }

  output += "@ " + std::to_string(R) + " C[" + color + "]\n";
  return output;
}
