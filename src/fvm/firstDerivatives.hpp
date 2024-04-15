#ifndef FIRSTDERIVATIVES_HPP
#define FIRSTDERIVATIVES_HPP

#include <cmath>
#include "grid.hpp"
#include "../geometry/vector.hpp"

template <typename var>
void firstDerivatives(const var& wl, const var& wr, const var& wA, const var& wB,
		    const Point2d& L, const Point2d& R, const Vector2d& s, Vector2<var>& gradW) {

  Vector2d LR(L, R);
  Vector2d sLR(LR.y, -LR.x);
  double volume = 0.5 * fabs(cross(s, sLR));

  gradW.x = 1. / (2. * volume) * ((wA - wB) * sLR.x + (wr - wl) * s.x);
  gradW.y = 1. / (2. * volume) * ((wA - wB) * sLR.y + (wr - wl) * s.y);
}

#endif
