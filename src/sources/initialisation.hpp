#ifndef INITIALISATION_HPP
#define INITIALISATION_HPP

#include <iostream>
#include <cstdlib>
#include "../fvm/cellField.hpp"
#include "fvm/limiter.hpp"
#include "fvm/grad.hpp"
#include "fvm/zeroLimiter.hpp"
#include "fvm/zeroGrad.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

void initialisation(CellField<Compressible>& w, const Setting& setting);

#endif
