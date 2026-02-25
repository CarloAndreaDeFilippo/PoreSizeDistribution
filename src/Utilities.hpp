#pragma once

#include <boost/filesystem.hpp>
#include <vector>

namespace fs = boost::filesystem;

inline bool directoryExists(const std::string& path) {
  return fs::exists(path) && fs::is_directory(path);
}

inline void makeDirectory(const std::string& path) {
  fs::create_directories(path);
}