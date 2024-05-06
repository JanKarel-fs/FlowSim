#include "primitiveVars.hpp"

PrimitiveVars PrimitiveVars::set(const Compressible& w) {
  double rho = w.rho;
  Vector2d u = w.rhoU / w.rho;
  double T = w.T();

  return PrimitiveVars(rho, u, T);
}
