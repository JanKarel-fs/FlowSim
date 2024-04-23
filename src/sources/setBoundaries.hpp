#ifndef SETBOUNDARIES_HPP
#define SETBOUNDARIES_HPP

#include <string>
#include "../fvm/grid.hpp"
#include "../fvm/cellField.hpp"
#include "../compressible.hpp"
#include "setting.hpp"
#include "typedefs.hpp"

using namespace std;

void setBoundaries(CellField<Compressible>& w, const Grid& g,
		   const Setting& setting, const map<string, bcWithJacobian>& BC);

#endif
