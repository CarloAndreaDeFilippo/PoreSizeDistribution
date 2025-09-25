#include "PoreSizeDistribution.hpp"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

void PoreSizeDistribution::initialize(const std::array<double, 3>& lbox) {
  Lbox = lbox;
  double maxDistance = 0.;

  for (auto& l : Lbox) maxDistance += l * l;

  maxDistance = sqrt(maxDistance);
  maxDistance *= 0.5;

  binNumber = (int)(maxDistance / binSize);

  poreSizeDistr.resize(binNumber, 0);
  oldPoreSizeDistr.resize(binNumber, 0);
}

void PoreSizeDistribution::insertData(const double diameter) {
  size_t bin = diameter / binSize;

  poreSizeDistr[bin]++;
}

void PoreSizeDistribution::insertSphere(const Sphere& sph) {
  poreSpheres.push_back(sph);
}

void PoreSizeDistribution::normalizeHistogram(size_t numSteps) {
  for (auto& bin : poreSizeDistr)
    bin /= ((double)numSteps);
}

void PoreSizeDistribution::saveHistogram(const std::string filename) {
  std::ofstream file_out;
  file_out.open(filename);

  for (size_t bin = 0; bin < binNumber; bin++) {
    file_out << bin * binSize << " " << poreSizeDistr[bin] << "\n";
  }

  file_out.close();
}

double PoreSizeDistribution::averageError() {
  double avError = 0.;

  for (size_t bin = 0; bin < poreSizeDistr.size(); bin++) {
    avError += std::abs(poreSizeDistr[bin] - oldPoreSizeDistr[bin]) / poreSizeDistr[bin];
  }

  return avError /= ((double)binNumber);
}

void PoreSizeDistribution::cogliPore(const std::string filename, std::string color, const bool append) {
  std::ofstream file_out;

  if (append == false) {
    file_out.open(filename);
  } else {
    file_out.open(filename, std::ios::app);
  }

  if (file_out.is_open() == false) {
    std::cout << "Error opening " << filename << "\n";
    std::exit(-1);
  }

  file_out << std::fixed << std::setprecision(16);

  for (auto& part : poreSpheres) {
    file_out << part.cogli2(color, Lbox);
  }

  file_out.close();
}
