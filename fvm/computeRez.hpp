#ifndef COMPUTEREZ_HPP
#define COMPUTEREZ_HPP

#include "cellField.hpp"
#include "grid.hpp"

template <typename var>
void computeRez(const CellField<var>& w, CellField<var>& rez, const Grid& g) {
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      rez[i][j].zero();
    }
  }

  // cyklus pres steny ve smeru i
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N()+1; j++) {
      var wl = w[i][j];
      var wr = w[i][j-1];

      Vector2d s = g.faceI(i, j).s;

      var flx = var::flux(wl, wr, s);

      rez[i][j] += flx;
      rez[i][j-1] -= flx;
    }
  }

  // cyklus pres steny ve smeru j
  for (int i=0; i<w.M()+1; i++) {
    for (int j=0; j<w.N(); j++) {
      var wl = w[i-1][j];
      var wr = w[i][j];

      Vector2d s = g.faceJ(i, j).s;

      var flx = var::flux(wl, wr, s);

      rez[i-1][j] += flx;
      rez[i][j] -= flx;
    }
  }

  // vydeleni objemem bunky a vynasobeni -1
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N(); j++) {
      rez[i][j] = -1. * rez[i][j] / g.volume(i, j);
    }
  }
}

#endif
