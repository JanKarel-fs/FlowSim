#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <utility>
#include "../geometry/vector.hpp"
#include "../geometry/matrix.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

typedef Compressible (*bCondition)(const Compressible& wInside, const Vector2d& s,
				     const Setting& setting);

typedef Matrixd (*bJacobian)(const Compressible& wInside, const Vector2d& s,
			     const Setting& setting);

typedef pair<bCondition, bJacobian> bcWithJacobian;

typedef pair<string, bcWithJacobian> condition;

#endif
