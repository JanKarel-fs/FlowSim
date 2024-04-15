#ifndef FIELD_HPP
#define FIELD_HPP

#include <iostream>
#include <cstdlib>

using namespace std;

template <typename T>
class Field2 {
  int imin;
  int imax;
  int jmin;
  int jmax;

  T *data;
  bool allocated;
  
public:
  Field2(): imin(0), imax(0), jmin(0), jmax(0) {allocated=false;};

  Field2(const Field2& fld2) {
    allocated = false;
    allocate(fld2.Imin(), fld2.Imax(), fld2.Jmin(), fld2.Jmax());

    int totalSize = (imax-imin) * (jmax-jmin);
    for (int i=0; i<totalSize; i++) {
      data[i] = fld2.data[i];
    }
  };
  
  Field2(int Imin, int Imax, int Jmin, int Jmax): imin(Imin), imax(Imax), jmin(Jmin), jmax(Jmax) {
    allocated = true;

    int totalSize = (imax-imin) * (jmax-jmin);

    data = new T[totalSize];
  };
  ~Field2() {
    if (allocated) {
      delete [] data;
    }
    allocated = false;
  };

  int Imin() const {return imin;}
  int Imax() const {return imax;}
  int Jmin() const {return jmin;}
  int Jmax() const {return jmax;}

  void allocate(const int& Imin, const int& Imax, const int& Jmin, const int& Jmax) {
    if (allocated) {
      cout << "Pole Field2 uz je naalokovane, pouzijte funkci reallocate!" << endl;
      exit(0);
    }

    allocated = true;

    imin = Imin;
    imax = Imax;
    jmin = Jmin;
    jmax = Jmax;

    int totalSize = (imax-imin) * (jmax-jmin);

    data = new T[totalSize];
  }

  void reallocate(const int& Imin, const int& Imax, const int& Jmin, const int& Jmax) {
    if (!allocated) {
      cout << "Pole Field2 jeste neni naalokovane, pouzijte funkci allocate!" << endl;
      exit(1);
    }

    delete [] data;

    allocated = false;

    allocate(Imin, Imax, Jmin, Jmax);
  }

  T* operator[](int i) const {
    int size_j = jmax-jmin;
    return data + (i-imin)*size_j - jmin;
  }

  Field2& operator=(const Field2& fld2) {
    if (allocated) {
      reallocate(fld2.Imin(), fld2.Imax(), fld2.Jmin(), fld2.Jmax());
    }
    else {
      allocate(fld2.Imin(), fld2.Imax(), fld2.Jmin(), fld2.Jmax());
    }

    int totalSize = (imax-imin) * (jmax-jmin);
    for (int i=0; i<totalSize; i++) {
      data[i] = fld2.data[i];
    }
  }
};
  

#endif
