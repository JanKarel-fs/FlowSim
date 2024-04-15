#include "compressible.hpp"

double Compressible::kappa;
double Compressible::R;
double Compressible::cp;
double Compressible::cv;
double Compressible::Pr;

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

double Compressible::T() const {
  return p() / (rho * R);
}

double Compressible::mu() const {
  return (1.45 * pow(T(), 3./2.)) / (T() + 110) * 1e-6;
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

Compressible Compressible::fluxDissipative(const Compressible& wFace,
					   const Vector2<PrimitiveVars>& gradPvars,
					   const Vector2d& s) {
  double divU = gradPvars.x.u.x + gradPvars.y.u.y;   // prvni .x znaci derivaci podle x, druhe .x znaci x-ovou
                                                     // slozku rychlosti

  double mu = wFace.mu();

  double Tau_xx = 2. * mu * (gradPvars.x.u.x - 1./3. * divU);
  double Tau_yy = 2. * mu * (gradPvars.y.u.y - 1./3. * divU);
  double Tau_xy = mu * (gradPvars.y.u.x + gradPvars.x.u.y);

  Vector2d firstRow(Tau_xx, Tau_xy);
  Vector2d secondRow(Tau_xy, Tau_yy);

  double k = cp * mu / Pr;
  Vector2d u = wFace.rhoU / wFace.rho;

  double Theta_x = dot(u, firstRow) + k * gradPvars.x.T;
  double Theta_y = dot(u, secondRow) + k * gradPvars.y.T;

  Vector2d thirdRow(Theta_x, Theta_y);

  double firstComponent = dot(firstRow, s);
  double secondComponent = dot(secondRow, s);
  double last = dot(thirdRow, s);
  
  return Compressible(0., Vector2d(firstComponent, secondComponent), last);
}
