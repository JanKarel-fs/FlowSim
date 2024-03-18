#ifndef INITIALISATION_HPP
#define INITIALISATION_HPP

#include <iostream>
#include <cstdlib>
#include "../fvm/cellField.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

void initialisation(CellField<Compressible>& w, const Setting& setting);

#endif
