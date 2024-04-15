#ifndef NOSLIPWALL_HPP
#define NOSLIPWALL_HPP

#include <cmath>
#include "../geometry/vector.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

Compressible noSlipWall(const Compressible& wInside, const Vector2d& s, const Setting& setting);

#endif
