#include "compressible.hpp"

double Compressible::kappa;

double Compressible::p() const {
  return (kappa - 1.) * (e - 0.5 * (rhoU.x*rhoU.x + rhoU.y*rhoU.y) / rho);
}

double Compressible::a() const {
  return std::sqrt(kapp * p() / rho);
}

double Compressible::Ma() const {
  return rhoU.length() / rho / a();
}
