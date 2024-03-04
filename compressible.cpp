#include "compressible.hpp"

double Compressible::kappa;

Compressible (*Compressible::flux)(const Compressible& wl, const Compressible& wr,
				   const Vector2d& s);

double Compressible::p() const {
  return (kappa - 1.) * (e - 0.5 * (rhoU.x*rhoU.x + rhoU.y*rhoU.y) / rho);
}

double Compressible::a() const {
  return std::sqrt(kapp * p() / rho);
}

double Compressible::Ma() const {
  return rhoU.length() / rho / a();
}

Compressible Compressible::fabs() {
  double RHO = std::fabs(rho);
  double RHOU = std::fabs(rhoU.x);
  double RHOV = std::fabs(rhoU.y);
  double E = std::fabs(e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}

Compressible Compressible::min() {
  double RHO = std::min(rho);
  double RHOU = std::min(rhoU.x);
  double RHOV = std::min(rhoU.y);
  double E = std::min(e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}

Compressible Compressible::max() {
  double RHO = std::max(rho);
  double RHOU = std::max(rhoU.x);
  double RHOV = std::max(rhoU.y);
  double E = std::max(e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}

Compressible Compressible::sqrt() {
  double RHO = std::sqrt(rho);
  double RHOU = std::sqrt(rhoU.x);
  double RHOV = std::sqrt(rhoU.y);
  double E = std::sqrt(e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}
