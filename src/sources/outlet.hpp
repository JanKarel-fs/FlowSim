#ifndef OUTLET_HPP
#define OUTLET_HPP

#include <cmath>
#include "../geometry/vector.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

Compressible outlet(const Compressible& wInside, const Vector2d& s,
		    const Setting& setting);

#endif
