#include "compressible.hpp"

double Compressible::kappa;

Compressible (*Compressible::flux)(const Compressible& wl, const Compressible& wr,
				   const Vector2d& s);

Compressible Compressible::Upwind(const Compressible& wl, const Compressible& wr,
				  const Vector2d& s) {
  Vector2d n = s / s.length();

  Vector2d u = (wl.rhoU/wl.rho + wr.rhoU/wr.rho) / 2.;

  double un = dot(u, n);

  Compressible flx;
  double p = (wl.p() + wr.p()) / 2.;

  if (un >= 0.) flx = wl * un;
  else flx = wr * un;

  flx += Compressible(0., p*n, p*un);

  return flx * s.length();
}


double Compressible::p() const {
  return (kappa - 1.) * (e - 0.5 * (rhoU.x*rhoU.x + rhoU.y*rhoU.y) / rho);
}

double Compressible::a() const {
  return std::sqrt(kappa * p() / rho);
}

double Compressible::Ma() const {
  return rhoU.length() / rho / a();
}

Compressible Compressible::fabs(const Compressible& a) {
  double RHO = std::fabs(a.rho);
  double RHOU = std::fabs(a.rhoU.x);
  double RHOV = std::fabs(a.rhoU.y);
  double E = std::fabs(a.e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}

Compressible Compressible::min(const Compressible& a,
			       const Compressible& b) {
  double RHO = std::min(a.rho, b.rho);
  double RHOU = std::min(a.rhoU.x, b.rhoU.x);
  double RHOV = std::min(a.rhoU.y, b.rhoU.y);
  double E = std::min(a.e, b.e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}

Compressible Compressible::max(const Compressible& a,
			       const Compressible& b) {
  double RHO = std::max(a.rho, b.rho);
  double RHOU = std::max(a.rhoU.x, b.rhoU.x);
  double RHOV = std::max(a.rhoU.y, b.rhoU.y);
  double E = std::max(a.e, b.e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}

Compressible Compressible::sqrt(const Compressible& a) {
  double RHO = std::sqrt(a.rho);
  double RHOU = std::sqrt(a.rhoU.x);
  double RHOV = std::sqrt(a.rhoU.y);
  double E = std::sqrt(a.e);

  return Compressible(RHO, Vector2d(RHOU, RHOV), E);
}
