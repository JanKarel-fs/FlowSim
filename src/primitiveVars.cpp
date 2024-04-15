#include "primitiveVars.hpp"

PrimitiveVars PrimitiveVars::set(const Compressible& w) {
  double p = w.p();
  Vector2d u = w.rhoU / w.rho;
  double T = w.T();

  return PrimitiveVars(p, u, T);
}
