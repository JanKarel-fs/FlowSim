#ifndef INLET_HPP
#define INLET_HPP

#include <cmath>
#include "../geometry/vector.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

Compressible inlet(const Compressible& wInside, const Vector2d& s,
		   const Setting& setting);

#endif
