#include "Cogli1.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>

void Cogli1::box(const std::array<double, 3>& Lbox, std::string filename) {
  std::ofstream file_out;

  file_out.open(filename);

  if (file_out.is_open() == false) {
    std::cout << "Error opening " << filename << "\n";
    std::exit(-1);
  }

  file_out << std::fixed << std::setprecision(16);

  file_out << ".Box: " << Lbox[0] << "," << Lbox[1] << "," << Lbox[2] << "\n";

  file_out.close();
}
