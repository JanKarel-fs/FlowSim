#include "noSlipWall.hpp"

Compressible noSlipWall(const Compressible& wInside, const Vector2d& s, const Setting& setting) {
  Compressible wg = wInside;
  wg.rhoU = -1. * wInside.rhoU;

  return wg;
}
