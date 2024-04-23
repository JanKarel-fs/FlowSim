#include "compressible.hpp"

double Compressible::kappa;
double Compressible::R;
double Compressible::cp;
double Compressible::cv;
double Compressible::Pr;

Compressible (*Compressible::flux)(const Compressible& wl, const Compressible& wr,
				   const Vector2d& s);

pair<pair<Matrixd, Matrixd>, Compressible> (*Compressible::fluxImplicit)(const Compressible& wl,
						      const Compressible& wr, const Vector2d& s);

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

Compressible Compressible::Rusanov(const Compressible& wl, const Compressible& wr, const Vector2d& s) {
  // Vector2d n = s / s.length();

  // double unL = dot(wl.rhoU/wl.rho, n);
  // double unR = dot(wr.rhoU/wr.rho, n);

  // double lambdaL = std::fabs(unL) + wl.a();
  // double lambdaR = std::fabs(unR) + wr.a();

  // double Slambda = std::max(lambdaL, lambdaR);

  // Compressible Fl(wl.rho*unL, wl.rhoU*unL + wl.p()*n, (wl.e + wl.p()) * unL);
  // Compressible Fr(wr.rho*unR, wr.rhoU*unR + wr.p()*n, (wr.e + wr.p()) * unR);

  // Compressible flx = 0.5 * (Fl + Fr) - 0.5 * Slambda * (wr - wl);

  // return flx * s.length();

  
  // Standardne: 1)rotace do n,t, 2)vypcoteni toku, 3) zpetna rotace do x,y
  Vector2d n = s / s.length();
  Vector2d t(-n.y, n.x);

  Compressible WR = wr;
  Compressible WL = wl;

  double rhouR = dot(wr.rhoU, n);
  double rhovR = dot(wr.rhoU, t);
  WR.rhoU = Vector2d(rhouR, rhovR);

  double rhouL = dot(wl.rhoU, n);
  double rhovL = dot(wl.rhoU, t);
  WL.rhoU = Vector2d(rhouL, rhovL);

  double lambdaL = std::fabs(WL.rhoU.x/WL.rho) + WL.a();
  double lambdaR = std::fabs(WR.rhoU.x/WR.rho) + WR.a();

  double Slambda = std::max(lambdaL, lambdaR);

  Compressible Fl(WL.rhoU.x, Vector2d(pow(WL.rhoU.x,2)/WL.rho + WL.p(), WL.rhoU.x*WL.rhoU.y/WL.rho),
		  (WL.e + WL.p()) * WL.rhoU.x/WL.rho);
  Compressible Fr(WR.rhoU.x, Vector2d(pow(WR.rhoU.x,2)/WR.rho + WR.p(), WR.rhoU.x*WR.rhoU.y/WR.rho),
		  (WR.e + WR.p()) * WR.rhoU.x/WR.rho);

  Compressible flx = 0.5 * (Fl + Fr) - 0.5 * Slambda * (WR - WL);

  Vector2d nInv(n.x, -n.y);
  Vector2d tInv(-t.x, t.y);

  double RHOU = dot(flx.rhoU, nInv);
  double RHOV = dot(flx.rhoU, tInv);

  flx.rhoU = Vector2d(RHOU, RHOV);

  return flx * s.length();
}

pair<pair<Matrixd, Matrixd>, Compressible> Compressible::UpwindImplicit(const Compressible& wl,
   						         const Compressible& wr, const Vector2d& s) {

  Compressible flx = Upwind(wl, wr, s);

  Matrixd JL(nVars), JR(nVars);
  JL.zero();  JR.zero();

  double ds = s.length();
  Vector2d n = s / ds;

  double p = (wl.p() + wr.p()) / 2.;

  double uL = wl.rhoU.x / wl.rho;
  double vL = wl.rhoU.y / wl.rho;
  double uLn = uL*n.x + vL*n.y;

  double uR = wr.rhoU.x / wr.rho;
  double vR = wr.rhoU.y / wr.rho;
  double uRn = uR*n.x + vR*n.y;

  double un = 0.5 * (uLn + uRn);

  if (un >= 0.) {
    // LEFT JACOBIAN
    // 1st row of the left Jacobian
    JL[0][0] = un - uLn/2.;  JL[0][1] = n.x/2.;  JL[0][2] = n.y/2.;

    // 2nd row of the left Jacobian
    JL[1][0] = -uL*uLn/2. + (kappa-1.)/4. * (uL*uL + vL*vL)*n.x;
    JL[1][1] = un + uL*n.x/2. - (kappa-1.)/2. * uL*n.x;
    JL[1][2] = uL*n.y/2. - (kappa-1.)/2. * vL*n.x;
    JL[1][3] = (kappa-1.)*n.x/2.;

    // 3rd row of the left Jacobian
    JL[2][0] = -vL*uLn/2. + (kappa-1.)/4. * (uL*uL + vL*vL)*n.y;
    JL[2][1] = vL*n.x/2. - (kappa-1.)/2. * uL*n.y;
    JL[2][2] = un + vL*n.y/2. - (kappa-1.)/2. * vL*n.y;
    JL[2][3] = (kappa-1.)*n.y/2.;

    // 4ht row of the left Jacobian
    JL[3][0] = (kappa-1.)/4. * (uL*uL + vL*vL)*un - (wl.e + p)/(2.*wl.rho) * uLn;
    JL[3][1] = -(kappa-1.)/2. * uL*un + (wl.e + p)/(2.*wl.rho) * n.x;
    JL[3][2] = -(kappa-1.)/2. * vL*un + (wl.e + p)/(2.*wl.rho) * n.y;
    JL[3][3] = (1. + (kappa-1.)/2.) * un;

    // RIGHT JACOBIAN
    // 1st row of the right Jacobian
    JR[0][0] = -0.5 * wl.rho/wr.rho * uRn;
    JR[0][1] = 0.5 * wl.rho/wr.rho * n.x;
    JR[0][2] = 0.5 * wl.rho/wr.rho * n.y;

    // 2nd row of the right Jacobian
    JR[1][0] = -wl.rhoU.x/wr.rho * uRn/2. + (kappa-1.)/4. * (uR*uR + vR*vR)*n.x;
    JR[1][1] = wl.rhoU.x/wr.rho * n.x/2. - (kappa-1.)/2. * uR*n.x;
    JR[1][2] = wl.rhoU.x/wr.rho * n.y/2. - (kappa-1.)/2. * vR*n.x;
    JR[1][3] = (kappa-1.)*n.x/2.;

    // 3rd row of the right Jacobian
    JR[2][0] = -wl.rhoU.y/wr.rho * uRn/2. + (kappa-1.)/4. * (uR*uR + vR*vR)*n.y;
    JR[2][1] = wl.rhoU.y/wr.rho * n.x/2. - (kappa-1.)/2. * uR*n.y;
    JR[2][2] = wl.rhoU.y/wr.rho * n.y/2. - (kappa-1.)/2. * vR*n.y;
    JR[2][3] = (kappa-1.)*n.y/2.;

    // 4th row of the right Jacobian
    JR[3][0] = (kappa-1.)/4. * (uR*uR + vR*vR)*un - (wl.e + p)/(2.*wr.rho) * uRn;
    JR[3][1] = -(kappa-1.)/2. * uR*un + (wl.e + p)/(2.*wr.rho) * n.x;
    JR[3][2] = -(kappa-1.)/2. * vR*un + (wl.e + p)/(2.*wr.rho) * n.y;
    JR[3][3] = (kappa-1.)*un/2.;
  }

  else {
    // LEFT JACOBIAN
    // 1st row of the left Jacobian
    JL[0][0] = -0.5 * wr.rho/wl.rho * uLn;
    JL[0][1] = 0.5 * wr.rho/wl.rho * n.x;
    JL[0][2] = 0.5 * wr.rho/wl.rho * n.y;

    // 2nd row of the left Jacobian
    JL[1][0] = -wr.rhoU.x/wl.rho * uLn/2. + (kappa-1.)/4. * (uL*uL + vL*vL)*n.x;
    JL[1][1] = wr.rhoU.x/wl.rho * n.x/2. - (kappa-1.)/2. * uL*n.x;
    JL[1][2] = wr.rhoU.x/wl.rho * n.y/2. - (kappa-1.)/2. * vL*n.x;
    JL[1][3] = (kappa-1.)*n.x/2.;

    // 3rd row of the left Jacobian
    JL[2][0] = -wr.rhoU.y/wl.rho * uLn/2. + (kappa-1.)/4. * (uL*uL + vL*vL)*n.y;
    JL[2][1] = wr.rhoU.y/wl.rho * n.x/2. - (kappa-1.)/2. * uL*n.y;
    JL[2][2] = wr.rhoU.y/wl.rho * n.y/2. - (kappa-1.)/2. * vL*n.y;
    JL[2][3] = (kappa-1.)*n.y/2.;

    // 4th row of the left Jacobian
    JL[3][0] = (kappa-1.)/4. * (uL*uL + vL*vL)*un - (wr.e + p)/(2.*wl.rho) * uLn;
    JL[3][1] = -(kappa-1.)/2. * uL*un + (wr.e + p)/(2.*wl.rho) * n.x;
    JL[3][2] = -(kappa-1.)/2. * vL*un + (wr.e + p)/(2.*wl.rho) * n.y;
    JL[3][3] = (kappa-1.)*un/2.;

    // RIGHT JACOBIAN
    // 1st row of the right Jacobian
    JR[0][0] = un - uRn/2.;  JR[0][1] = n.x/2.;  JR[0][2] = n.y/2.;

    // 2nd row of the right Jacobian
    JR[1][0] = -uR*uRn/2. + (kappa-1.)/4. * (uR*uR + vR*vR)*n.x;
    JR[1][1] = un + uR*n.x/2. - (kappa-1.)/2. * uR*n.x;
    JR[1][2] = uR*n.y/2. - (kappa-1.)/2. * vR*n.x;
    JR[1][3] = (kappa-1.)*n.x/2.;

    // 3rd row of the right Jacobian
    JR[2][0] = -vR*uRn/2. + (kappa-1.)/4. * (uR*uR + vR*vR)*n.y;
    JR[2][1] = vR*n.x/2. - (kappa-1.)/2. * uR*n.y;
    JR[2][2] = un + vR*n.y/2. - (kappa-1.)/2. * vR*n.y;
    JR[2][3] = (kappa-1.)*n.y/2.;

    // 4ht row of the right Jacobian
    JR[3][0] = (kappa-1.)/4. * (uR*uR + vR*vR)*un - (wr.e + p)/(2.*wr.rho) * uRn;
    JR[3][1] = -(kappa-1.)/2. * uR*un + (wr.e + p)/(2.*wr.rho) * n.x;
    JR[3][2] = -(kappa-1.)/2. * vR*un + (wr.e + p)/(2.*wr.rho) * n.y;
    JR[3][3] = (1. + (kappa-1.)/2.) * un;
  }

  pair<pair<Matrixd, Matrixd>, Compressible> JacobiansAndRHS;

  JacobiansAndRHS.first.first = JL * ds;
  JacobiansAndRHS.first.second = JR * ds;
  JacobiansAndRHS.second = flx;

  return JacobiansAndRHS;
}


pair<pair<Matrixd, Matrixd>, Compressible> Compressible::RusanovImplicit(const Compressible& wl,
							 const Compressible& wr, const Vector2d& s) {


  Compressible flx = Rusanov(wl, wr, s);

  Matrixd JL(nVars), JR(nVars);
  JL.zero();  JR.zero();

  double ds = s.length();
  Vector2d n = s / ds;
  Vector2d t(-n.y, n.x);

  Compressible WR = wr;
  Compressible WL = wl;

  double rhouR = dot(wr.rhoU, n);
  double rhovR = dot(wr.rhoU, t);
  WR.rhoU = Vector2d(rhouR, rhovR);

  double rhouL = dot(wl.rhoU, n);
  double rhovL = dot(wl.rhoU, t);
  WL.rhoU = Vector2d(rhouL, rhovL);

  double lambdaL = std::fabs(WL.rhoU.x/WL.rho) + WL.a();
  double lambdaR = std::fabs(WR.rhoU.x/WR.rho) + WR.a();

  double Slambda = std::max(lambdaL, lambdaR);

  double rhoL = WL.rho;
  double uL = WL.rhoU.x / WL.rho;
  double vL = WL.rhoU.y / WL.rho;
  double eL = WL.e;
  double pL = WL.p();

  double rhoR = WR.rho;
  double uR = WR.rhoU.x / WR.rho;
  double vR = WR.rhoU.y / WR.rho;
  double eR = WR.e;
  double pR = WR.p();


  // LEFT JACOBIAN
  // 1st row of the left Jacobian
  JL[0][1] = 1.;

  // 2nd row of the left Jacobian
  JL[1][0] = -uL*uL + (kappa-1.)/2. * (uL*uL + vL*vL);
  JL[1][1] = 2.*uL - (kappa-1.) * uL;
  JL[1][2] = -(kappa-1.) * vL;
  JL[1][3] = (kappa-1.);

  // 3rd row of the left Jacobian
  JL[2][0] = -uL*vL;
  JL[2][1] = vL;
  JL[2][2] = uL;

  // 4ht row of the left Jacobian
  JL[3][0] = (kappa-1.)/2. * (uL*uL + vL*vL)*uL - (eL + pL)/(rhoL) * uL;
  JL[3][1] = -(kappa-1.) * uL*uL + (eL + pL)/(rhoL);
  JL[3][2] = -(kappa-1.) * vL*uL;
  JL[3][3] = kappa * uL;

  // RIGHT JACOBIAN
  // 1st row of the right Jacobian
  JR[0][1] = 1.;

  // 2nd row of the right Jacobian
  JR[1][0] = -uR*uR + (kappa-1.)/2. * (uR*uR + vR*vR);
  JR[1][1] = 2.*uR - (kappa-1.) * uR;
  JR[1][2] = -(kappa-1.) * vR;
  JR[1][3] = (kappa-1.);

  // 3rd row of the right Jacobian
  JR[2][0] = -uR*vR;
  JR[2][1] = vR;
  JR[2][2] = uR;

  // 4th row of the right Jacobian
  JR[3][0] = (kappa-1.)/2. * (uR*uR + vR*vR)*uR - (eR + pR)/(rhoR) * uR;
  JR[3][1] = -(kappa-1.) * uR*uR + (eR + pR)/(rhoR);
  JR[3][2] = -(kappa-1.) * vR*uR;
  JR[3][3] = kappa * uR;

  Matrixd I(nVars);
  I.ones();

  JL = 0.5 * JL + Slambda/2. * I;
  JR = 0.5 * JR - Slambda/2. * I;

  Vector2d nInv(n.x, -n.y);
  Vector2d tInv(-t.x, t.y);

  Matrixd T(nVars), Tinv(nVars);
  T.ones();
  Tinv.ones();

  T[1][1] = n.x;  T[1][2] = n.y;
  T[2][1] = t.x;  T[2][2] = t.y;

  Tinv[1][1] = nInv.x;  Tinv[1][2] = nInv.y;
  Tinv[2][1] = tInv.x;  Tinv[2][2] = tInv.y;

  pair<pair<Matrixd, Matrixd>, Compressible> JacobiansAndRHS;

  JacobiansAndRHS.first.first = Tinv *JL * T * ds;
  JacobiansAndRHS.first.second = Tinv * JR * T * ds;
  JacobiansAndRHS.second = flx;

  return JacobiansAndRHS;
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
