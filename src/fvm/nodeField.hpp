#ifndef NODEFIELD_HPP
#define NODEFIELD_HPP

#include <iostream>
#include <cstdlib>
#include "grid.hpp"
#include "../geometry/field.hpp"

using namespace std;

template <typename T>
class NodeField {
  Field2<T> data;

  int Mnd;
  int Nnd;
  int ghost;

public:
  NodeField(int M_, int N_, int gh): Mnd(M_), Nnd(N_), ghost(gh) {
    data.allocate(-ghost, Mnd+ghost, -ghost, Nnd+ghost);
  };

  NodeField(const Grid& g) {
    Mnd = g.Mnd();
    Nnd = g.Nnd();
    ghost = g.gh();

    data.allocate(-ghost, Mnd+ghost, -ghost, Nnd+ghost);
  };

  ~NodeField() {};

  int M() const {return Mnd;}
  int N() const {return Nnd;}
  int gh() const {return ghost;}
  int Imin() const {return data.Imin();}
  int Imax() const {return data.Imax();}
  int Jmin() const {return data.Jmin();}
  int Jmax() const {return data.Jmax();}

  inline T* operator[](int i) const {
    return data[i];
  }

  NodeField operator=(const NodeField& cf) {
    if (Mnd!=cf.M() || Nnd!=cf.N() || ghost!=cf.gh()) {
      cout << "Nelze priradit hodnoty z pole NodeField jine velikosti!"
	   << endl;
      exit(2);
    }

    for (int i=Imin(); i<Imax(); i++) {
      for (int j=Jmin(); j<Jmax(); j++) {
	data[i][j] = cf[i][j];
      }
    }

    return *this;
  }
};

template <typename T>
inline NodeField<T> operator+(const NodeField<T>& a, const NodeField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze scitat pole NodeField ruznych velikosti!" << endl;
    exit(3);
  }

  NodeField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] + b[i][j];
    }
  }

  return c;
}

template <typename T>
inline NodeField<T> operator-(const NodeField<T>& a, const NodeField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze odecitat pole NodeField ruznych velikosti!" << endl;
    exit(3);
  }

  NodeField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] - b[i][j];
    }
  }

  return c;
}

template <typename T, typename S>
inline NodeField<T> operator*(const NodeField<T>& a, const S& b) {
  NodeField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] * b;
    }
  }

  return c;
}

template <typename T, typename S>
inline NodeField<T> operator*(const S& b, const NodeField<T>& a) {
  NodeField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] * b;
    }
  }

  return c;
}

template <typename T, typename S>
inline NodeField<T> operator/(const NodeField<T>& a, const S& b) {
  NodeField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] / b;
    }
  }

  return c;
}

template <typename T>
inline NodeField<T> operator+=(NodeField<T>& a, const NodeField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze scitat (+=) pole NodeField ruznych velikosti!" << endl;
    exit(4);
  }

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      a[i][j] += b[i][j];
    }
  }

  return a;
}

template <typename T>
inline NodeField<T> operator-=(NodeField<T>& a, const NodeField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze odecitat (-=) pole NodeField ruznych velikosti!" << endl;
    exit(3);
  }

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      a[i][j] -= b[i][j];
    }
  }

  return a;
}

template <typename T, typename S>
inline NodeField<T> operator*=(NodeField<T>& a, const S& b) {

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      a[i][j] *= b;
    }
  }

  return a;
}

template <typename T, typename S>
inline NodeField<T> operator/=(NodeField<T>& a, const S& b) {

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      a[i][j] /= b;
    }
  }

  return a;
}
  
#endif
