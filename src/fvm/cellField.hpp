#ifndef CELLFIELD_HPP
#define CELLFIELD_HPP

#include <iostream>
#include <cstdlib>
#include "grid.hpp"
#include "../geometry/field.hpp"

using namespace std;

template <typename T>
class CellField {
  Field2<T> data;

  int Mvol;
  int Nvol;
  int ghost;

public:
  CellField(int M_, int N_, int gh): Mvol(M_), Nvol(N_), ghost(gh) {
    data.allocate(-ghost, Mvol+ghost, -ghost, Nvol+ghost);
  };

  CellField(const Grid& g) {
    Mvol = g.Mvol();
    Nvol = g.Nvol();
    ghost = g.gh();

    data.allocate(-ghost, Mvol+ghost, -ghost, Nvol+ghost);
  };

  ~CellField() {};

  int M() const {return Mvol;}
  int N() const {return Nvol;}
  int gh() const {return ghost;}
  int Imin() const {return data.Imin();}
  int Imax() const {return data.Imax();}
  int Jmin() const {return data.Jmin();}
  int Jmax() const {return data.Jmax();}

  inline T* operator[](int i) const {
    return data[i];
  }

  CellField operator=(const CellField& cf) {
    if (Mvol!=cf.M() || Nvol!=cf.N() || ghost!=cf.gh()) {
      cout << "Nelze priradit hodnoty z pole CellField jine velikosti!"
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
inline CellField<T> operator+(const CellField<T>& a, const CellField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze scitat pole CellField ruznych velikosti!" << endl;
    exit(3);
  }

  CellField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] + b[i][j];
    }
  }

  return c;
}

template <typename T>
inline CellField<T> operator-(const CellField<T>& a, const CellField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze odecitat pole CellField ruznych velikosti!" << endl;
    exit(3);
  }

  CellField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] - b[i][j];
    }
  }

  return c;
}

template <typename T, typename S>
inline CellField<T> operator*(const CellField<T>& a, const S& b) {
  CellField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] * b;
    }
  }

  return c;
}

template <typename T, typename S>
inline CellField<T> operator*(const S& b, const CellField<T>& a) {
  CellField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] * b;
    }
  }

  return c;
}

template <typename T, typename S>
inline CellField<T> operator/(const CellField<T>& a, const S& b) {
  CellField<T> c(a.M(), a.N(), a.gh());

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      c[i][j] = a[i][j] / b;
    }
  }

  return c;
}

template <typename T>
inline CellField<T> operator+=(CellField<T>& a, const CellField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze scitat (+=) pole CellField ruznych velikosti!" << endl;
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
inline CellField<T> operator-=(CellField<T>& a, const CellField<T>& b) {
  if (a.M()!=b.M() || a.N()!=b.N() || a.gh()!=b.gh()) {
    cout << "Nelze odecitat (-=) pole CellField ruznych velikosti!" << endl;
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
inline CellField<T> operator*=(CellField<T>& a, const S& b) {

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      a[i][j] *= b;
    }
  }

  return a;
}

template <typename T, typename S>
inline CellField<T> operator/=(CellField<T>& a, const S& b) {

  for (int i=a.Imin(); i<a.Imax(); i++) {
    for (int j=a.Jmin(); j<a.Jmax(); j++) {
      a[i][j] /= b;
    }
  }

  return a;
}
  
#endif
