#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <utility>
#include "../geometry/vector.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

typedef Compressible (*bCondition)(const Compressible& wInside, const Vector2d& s,
				     const Setting& setting);

typedef pair<string, bCondition> condition;

#endif
