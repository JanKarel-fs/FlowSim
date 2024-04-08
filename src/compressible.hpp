#ifndef COMPRESSIBLE_HPP
#define COMPRESSIBLE_HPP

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include "geometry/vector.hpp"

using namespace std;

class Compressible {
public:
  double rho;
  Vector2d rhoU;
  double e;

  Compressible() {};
  Compressible(const double& RHO, const Vector2d& RHOU, const double& E):
    rho(RHO), rhoU(RHOU), e(E) {};
  ~Compressible() {};

  static double kappa;

  inline void zero() {
    rho=0.; rhoU = Vector2d(0., 0.); e=0.;
  }

  inline void ones() {
    rho=1.; rhoU = Vector2d(1., 1.); e=1.;
  }

  double& operator[](int k) {
    switch (k) {
    case 0: return rho;
      break;
    case 1: return rhoU.x;
      break;
    case 2: return rhoU.y;
      break;
    case 3: return e;
      break;
    default: cout << "Class compressible: out of a range of an operator []!" << endl;
      exit(11);
    }
  }

  double operator[](int k) const {
    switch (k) {
    case 0: return rho;
      break;
    case 1: return rhoU.x;
      break;
    case 2: return rhoU.y;
      break;
    case 3: return e;
      break;
    default: cout << "Class compressible: out of a range of an operator []!" << endl;
      exit(11);
    }
  }

  static const int nVars = 4;

  static Compressible (*flux)(const Compressible& wl, const Compressible& wr,
			      const Vector2d& s);
  
  static Compressible Upwind(const Compressible& wl, const Compressible& wr,
			     const Vector2d& s);
  
  double p() const;
  double a() const;
  double Ma() const;

  static Compressible fabs(const Compressible& a);
  static Compressible min(const Compressible& a,
			  const Compressible& b);
  static Compressible max(const Compressible& a,
			  const Compressible& b);
  static Compressible sqrt(const Compressible& a);
};

inline Compressible operator+(const Compressible& a, const Compressible& b) {
  return Compressible(a.rho+b.rho, a.rhoU+b.rhoU, a.e+b.e);
}

inline Compressible operator-(const Compressible& a, const Compressible& b) {
  return Compressible(a.rho-b.rho, a.rhoU-b.rhoU, a.e-b.e);
}

inline Compressible operator*(const Compressible& a, const Compressible& b) {
  return Compressible(a.rho*b.rho, a.rhoU*b.rhoU, a.e*b.e);
}

template <typename S>
inline Compressible operator*(const Compressible& a, const S& b) {
  return Compressible(a.rho*b, a.rhoU*b, a.e*b);
}

template <typename S>
inline Compressible operator*(const S& b, const Compressible& a) {
  return Compressible(a.rho*b, a.rhoU*b, a.e*b);
}

template <typename S>
inline Compressible operator/(const Compressible& a, const S& b) {
  return Compressible(a.rho/b, a.rhoU/b, a.e/b);
}

inline Compressible operator+=(Compressible& a, const Compressible& b) {
  a.rho+=b.rho; a.rhoU+=b.rhoU; a.e+=b.e;
  return a;
}

inline Compressible operator-=(Compressible& a, const Compressible& b) {
  a.rho-=b.rho; a.rhoU-=b.rhoU; a.e-=b.e;
  return a;
}

inline Compressible operator*=(Compressible& a, const Compressible& b) {
  a.rho*=b.rho; a.rhoU*=b.rhoU; a.e*=b.e;
  return a;
}

template <typename S>
inline Compressible operator*=(Compressible& a, const S& b) {
  a.rho*=b; a.rhoU*=b; a.e*=b;
  return a;
}

template <typename S>
inline Compressible operator/=(Compressible& a, const S& b) {
  a.rho/=b; a.rhoU/=b; a.e/=b;
  return a;
}

#endif
