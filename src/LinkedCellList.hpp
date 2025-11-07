#pragma once

#include <algorithm>
#include <array>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <vector>

template <class Object>
class LinkedCellList {
 public:
  std::array<double, 3> Lbox;
  double cellMinWidth = 1.;
  std::array<double, 3> cellWidth = {{1., 1., 1.}};
  std::vector<int> cellsPerAxis = {{1, 1, 1}};
  std::vector<int> maxCellsPerAxis = {{200, 200, 200}};
  size_t totCells = 0;
  std::array<int, 3> neighboringIndexes = {{2, 2, 2}};

  std::vector<int> head;
  std::vector<int> list;

  LinkedCellList() = default;
  LinkedCellList(std::array<double, 3> lbox) {
    Lbox = lbox;
  }

  void setCellMinWidth(double minWidth);
  std::array<int, 4> calculateObjectCell(int obj, const std::vector<Object>& objects);
  void createList(const std::vector<Object>& objects);
  double minDistance(const std::array<double, 3>& cm, const std::vector<Object>& objects) const;
  void printList();
};

template <class Object>
inline void LinkedCellList<Object>::setCellMinWidth(double minWidth) {
  cellMinWidth = minWidth;
}

template <class Object>
inline std::array<int, 4> LinkedCellList<Object>::calculateObjectCell(int obj, const std::vector<Object>& objects) {
  std::array<int, 4> cellIndex = {0, 0, 0, 0};

  for (int i = 0; i < 3; i++) cellIndex[i] = (objects[obj].tf.cm[i] + 0.5 * Lbox[i]) / cellWidth[i];

  cellIndex[3] = cellIndex[0] + cellIndex[1] * cellsPerAxis[0] + cellIndex[2] * cellsPerAxis[0] * cellsPerAxis[1];

  return cellIndex;
}

template <class Object>
inline void LinkedCellList<Object>::createList(const std::vector<Object>& objects) {
  for (int i = 0; i < 3; i++) {
    cellsPerAxis[i] = Lbox[i] / cellMinWidth;

    if (cellsPerAxis[i] > maxCellsPerAxis[i]) cellsPerAxis[i] = maxCellsPerAxis[i];

    cellWidth[i] = Lbox[i] / ((double)cellsPerAxis[i]);
  }

  totCells = cellsPerAxis[0] * cellsPerAxis[1] * cellsPerAxis[2];

  if (totCells > head.size()) head.resize(totCells);

  if (list.size() < objects.size()) list.resize(objects.size());

  for (auto& cell : head) cell = -1;
  for (auto& cell : list) cell = -1;

  // Linked Cell List construction

  for (size_t obj = 0; obj < objects.size(); obj++) {
    std::array<int, 4> cellIndex = calculateObjectCell(obj, objects);

    list[obj] = head[cellIndex[3]];
    head[cellIndex[3]] = obj;
  }

  // If the volume is too little to have 3 cells in each direction
  for (int ax = 0; ax < 3; ax++)
    if (cellsPerAxis[ax] < 3) neighboringIndexes[ax] = cellsPerAxis[ax] - 1;
}

template <class Object>
inline double LinkedCellList<Object>::minDistance(const std::array<double, 3>& cm, const std::vector<Object>& objects) const {
  double minDist = DBL_MAX;
  std::array<int, 4> cellIndexCM = {0, 0, 0, 0};

  for (int i = 0; i < 3; i++) cellIndexCM[i] = (cm[i] + 0.5 * Lbox[i]) / cellWidth[i];

  cellIndexCM[3] = cellIndexCM[0] + cellIndexCM[1] * cellsPerAxis[0] + cellIndexCM[2] * cellsPerAxis[0] * cellsPerAxis[1];

  std::array<int, 4> cellIndex = {0, 0, 0, 0};

  for (int kx = -1; kx < neighboringIndexes[0]; kx++) {
    for (int ky = -1; ky < neighboringIndexes[1]; ky++) {
      for (int kz = -1; kz < neighboringIndexes[2]; kz++) {
        cellIndex[0] = cellIndexCM[0] + kx;
        cellIndex[1] = cellIndexCM[1] + ky;
        cellIndex[2] = cellIndexCM[2] + kz;

        // PBC

        for (int ax = 0; ax < 3; ax++) {
          if (cellIndex[ax] >= cellsPerAxis[ax]) {
            cellIndex[ax] -= cellsPerAxis[ax];
          } else if (cellIndex[ax] < 0)
            cellIndex[ax] += cellsPerAxis[ax];
        }

        cellIndex[3] = cellIndex[0] + cellIndex[1] * cellsPerAxis[0] + cellIndex[2] * cellsPerAxis[0] * cellsPerAxis[1];

        cellIndex[3] = head[cellIndex[3]];

        while (cellIndex[3] >= 0) {
          minDist = std::min(minDist, objects[cellIndex[3]].minDistance(cm, Lbox));

          cellIndex[3] = list[cellIndex[3]];
        }
      }
    }
  }

  return minDist;
}

template <class Object>
inline void LinkedCellList<Object>::printList() {
  for (size_t cell = 0; cell < head.size(); cell++) {
    int c = head[cell];

    if (c == -1) continue;

    std::cout << cell << ": ";

    do {
      std::cout << c << " ";
      c = list[c];

    } while (c >= 0);

    std::cout
        << "-1\n";
  }
}
