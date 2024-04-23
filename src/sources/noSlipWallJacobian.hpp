#ifndef NOSLIPWALLJACOBIAN_HPP
#define NOSLIPWALLJACOBIAN_HPP

#include <cmath>
#include "../geometry/vector.hpp"
#include "../geometry/matrix.hpp"
#include "../compressible.hpp"
#include "setting.hpp"

using namespace std;

Matrixd noSlipWallJacobian(const Compressible& wInside, const Vector2d& s, const Setting& setting);

#endif
