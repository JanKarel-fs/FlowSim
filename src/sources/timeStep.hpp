#ifndef TIMESTEP_HPP
#define TIMESTEP_HPP

#include <cmath>
#include "../fvm/grid.hpp"
#include "../fvm/cellField.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

double timeStep(const CellField<Compressible>& w, const Grid& g,
		Setting& setting);

#endif
