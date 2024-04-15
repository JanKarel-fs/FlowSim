#ifndef COMPUTEREZ_HPP
#define COMPUTEREZ_HPP

#include <omp.h>
#include "cellField.hpp"
#include "grid.hpp"
#include "computeResidualConv.hpp"
#include "computeResidualDiss.hpp"
#include "../geometry/vector.hpp"
#include "sources/setting.hpp"

template <typename var>
void computeRez(const CellField<var>& w, CellField<var>& rez, const Grid& g, const Setting& setting) {
#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      rez[i][j].zero();
    }
  }

  switch (setting.convection) {
  case 0: break;      // konvekce je vypnuta nedelame nic
  case 1: computeResidualConv(w, rez, g);
    break;
  default: cout << "No such possibility for convection!" << endl;
    exit(15);
  }

  switch (setting.diffusion) {
  case 0: break;      // difuze je vypnuta nedelame nic
  case 1: computeResidualDiss(w, rez, g);
    break;
  default: cout << "No such possibility for convection!" << endl;
    exit(15);
  }
  
#pragma omp parallel for
  // vydeleni objemem bunky
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N(); j++) {
      rez[i][j] = rez[i][j] / g.volume(i, j);
    }
  }
}

#endif
