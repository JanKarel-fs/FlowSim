#ifndef SAVERESULTS_HPP
#define SAVERESULTS_HPP

#include <fstream>
#include "../fvm/cellField.hpp"
#include "../fvm/nodeField.hpp"
#include "../fvm/grid.hpp"
#include "../compressible.hpp"
#include "vtkOutput.hpp"

using namespace std;

void saveResults(const CellField<Compressible>& w, const Grid& g);

#endif
