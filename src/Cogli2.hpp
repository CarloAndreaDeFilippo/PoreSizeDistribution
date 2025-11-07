#pragma once

#include <array>
#include <string>
#include <vector>

namespace Cogli2 {

// Definition of simulation box Cogli2 file
void box(const std::array<double, 3>& L, const std::string& filename);

}  // namespace Cogli2