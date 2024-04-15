#ifndef SYMMETRY_HPP
#define SYMMETRY_HPP

#include <cmath>
#include "../geometry/vector.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

Compressible symmetry(const Compressible& wInside, const Vector2d& s, const Setting& setting);

#endif
