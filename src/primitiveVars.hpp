#ifndef PRIMITIVEVARS_HPP
#define PRIMITIVEVARS_HPP

#include <cmath>
#include <algorithm>
#include "geometry/vector.hpp"
#include "compressible.hpp"

using namespace std;

class Compressible;

class PrimitiveVars {
public:
  double rho;
  Vector2d u;
  double T;

  PrimitiveVars() {};
  PrimitiveVars(const double& RHO, const Vector2d& U, const double& t): rho(RHO), u(U), T(t) {};
  ~PrimitiveVars() {};

  void zero() {
    rho = 0.; u = Vector2d(0., 0.); T = 0.;
  }

  void ones() {
    rho = 1.; u = Vector2d(1., 1.); T = 1.;
  }

  static PrimitiveVars set(const Compressible& w);

  static const int nVars = 4;

  inline double operator[](int k) const {
    switch (k) {
    case 0: return rho;
      break;
    case 1: return u.x;
      break;
    case 2: return u.y;
      break;
    case 3: return T;
      break;
    }
  }

  inline double& operator[](int k) {
    switch (k) {
    case 0: return rho;
      break;
    case 1: return u.x;
      break;
    case 2: return u.y;
      break;
    case 3: return T;
      break;
    }
  }
};

inline PrimitiveVars operator+(const PrimitiveVars& a, const PrimitiveVars& b) {
  return PrimitiveVars(a.rho+b.rho, a.u+b.u, a.T+b.T);
}

inline PrimitiveVars operator-(const PrimitiveVars& a, const PrimitiveVars& b) {
  return PrimitiveVars(a.rho-b.rho, a.u-b.u, a.T-b.T);
}

inline PrimitiveVars operator*(const PrimitiveVars& a, const PrimitiveVars& b) {
  return PrimitiveVars(a.rho*b.rho, a.u*b.u, a.T*b.T);
}

template <typename S>
inline PrimitiveVars operator*(const PrimitiveVars& a, const S& b) {
  return PrimitiveVars(a.rho*b, a.u*b, a.T*b);
}

template <typename S>
inline PrimitiveVars operator*(const S& b, const PrimitiveVars& a) {
  return PrimitiveVars(a.rho*b, a.u*b, a.T*b);
}

template <typename S>
inline PrimitiveVars operator/(const PrimitiveVars& a, const S& b) {
  return PrimitiveVars(a.rho/b, a.u/b, a.T/b);
}

inline PrimitiveVars operator+=(PrimitiveVars& a, const PrimitiveVars& b) {
  a.rho+=b.rho; a.u+=b.u; a.T+=b.T;
  return a;
}

inline PrimitiveVars operator-=(PrimitiveVars& a, const PrimitiveVars& b) {
  a.rho-=b.rho; a.u-=b.u; a.T-=b.T;
  return a;
}

template <typename S>
inline PrimitiveVars operator*=(PrimitiveVars& a, const S& b) {
  a.rho*=b; a.u*=b; a.T*=b;
  return a;
}

template <typename S>
inline PrimitiveVars operator/=(PrimitiveVars& a, const S& b) {
  a.rho/=b; a.u/=b; a.T/=b;
  return a;
}

#endif
