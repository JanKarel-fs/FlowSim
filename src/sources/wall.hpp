#ifndef WALL_HPP
#define WALL_HPP

#include <cmath>
#include "../geometry/vector.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

Compressible wall(const Compressible& wInside, const Vector2d& s,
		  const Setting& setting);

#endif
