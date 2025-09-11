#include "ParticleSystem.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

void ParticleSystem::addParticle(const Sphere& part) {
  particles.push_back(part);
  N++;
}

void ParticleSystem::loadSystem(const std::string infile) {
  std::string line;

  std::ifstream file_in(infile);

  if (file_in.is_open() == false) {
    std::cout << "Error opening " << infile << "\n";
    std::exit(-1);
  }

  // First line for the box length
  if (std::getline(file_in, line)) {
    std::istringstream iss(line);

    iss >> Lbox[0] >> Lbox[1] >> Lbox[2];
  }

  while (std::getline(file_in, line)) {
    std::istringstream iss(line);

    Sphere sph;

    iss >> sph.tf.cm[0] >> sph.tf.cm[1] >> sph.tf.cm[2];

    addParticle(sph);
  }
}

void ParticleSystem::cogli1(const std::string filename, std::string color, const bool append) {
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

  for (auto& part : particles) {
    file_out << part.cogli1(color, Lbox);
  }

  file_out.close();
}
