#ifndef SAVENORMRESIDUAL_HPP
#define SAVENORMRESIDUAL_HPP

#include <fstream>
#include "../fvm/cellField.hpp"
#include "../fvm/grid.hpp"
#include "../compressible.hpp"

void saveNormResidual(const CellField<Compressible>& rez,
		      const Grid& g, const int& iter);

#endif
