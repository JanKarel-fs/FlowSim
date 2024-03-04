#include "outlet.hpp"

Compressible outlet(const Compressible& wInside, const Vector2d& s,
		    const Setting& setting) {
  const double& M2is = setting.M2is;
  const double& p0 = setting.p0;
  const double& kappa = Compressible::kappa;

  const double& rho = wInside.rho;
  const Vector2d& rhoU = wInside.rhoU;

  double p = p0 * pow((kappa-1.)/2. * M2is*M2is + 1, kappa/(1.-kappa));

  double e = p/(kappa-1.) + 0.5 * (rhoU.x*rhoU.x + rhoU.y*rhoU.y) / rho;

  return Compressible(rho, rhoU, e);
}
