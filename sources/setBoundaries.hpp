#ifndef SETBOUNDARIES_HPP
#define SETBOUNDARIES_HPP

#include "../fvm/grid.hpp"
#include "../fvm/cellField.hpp"
#include "../compressible"
#include "setting.hpp"
#include "inlet.hpp"
#include "outlet.hpp"
#include "wall.hpp"

void setBoundaries(CellField<Compressible>& w, const Grid& g, const Setting& setting);

#endif
