#include "inlet.hpp"

Compressible inlet(const Compressible& wInside, const Vector2d& s,
		   const Setting& setting) {
  const double& p0 = setting.p0;
  const double& rho0 = setting.rho0;
  const double& alpha = setting.alpha;
  const double& kappa = Compressible::kappa;

  double p = wInside.p();

  double rho = pow(p/p0, 1./kappa) * rho0;
  double Ma = sqrt(2./(kappa - 1.) * (pow(p/p0, (1.-kappa)/kappa) - 1.));
  double a = sqrt(kappa * p / rho);

  double rhou = rho * Ma * a * cos(alpha);
  double rhov = rho * Ma * a * sin(alpha);

  double e = p / (kappa - 1.) + 0.5 * (rhou*rhou + rhov*rhov) / rho;

  return Compressible(rho, Vector2d(rhou, rhov), e);
}
