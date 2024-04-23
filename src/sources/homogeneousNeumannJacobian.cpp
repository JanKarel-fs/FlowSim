#include "homogeneousNeumannJacobian.hpp"

Matrixd homogeneousNeumannJacobian(const Compressible& wInside, const Vector2d& s, const Setting& setting) {
  Matrixd J(Compressible::nVars);
  J.ones();
  
  return J;
}
