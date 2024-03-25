#ifndef COMPUTEREZ_HPP
#define COMPUTEREZ_HPP

#include <omp.h>
#include "cellField.hpp"
#include "grid.hpp"
#include "grad.hpp"
#include "limiter.hpp"

template <typename var>
void computeRez(const CellField<var>& w, CellField<var>& rez, const Grid& g) {
  CellField<var> psi(g);
  CellField<Vector2<var> > gradW(g);

  grad<var>(w, gradW, g);
  limiter<var>(w, gradW, psi, g);
  
#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      rez[i][j].zero();
    }
  }

#pragma omp parallel for
  // cyklus pres steny ve smeru i
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N()+1; j++) {
      
      Face f = g.faceI(i, j);
      Vector2d rL(g.center(i, j), f.center);
      Vector2d rR(g.center(i, j-1), f.center);
      
      var wl = w[i][j] + psi[i][j]
	     * (gradW[i][j].x * rL.x + gradW[i][j].y * rL.y);
      
      var wr = w[i][j-1] + psi[i][j-1]
	     * (gradW[i][j-1].x * rR.x + gradW[i][j-1].y * rR.y);

      var flx = var::flux(wl, wr, f.s);

      rez[i][j] += flx;
      rez[i][j-1] -= flx;
    }
  }

#pragma omp parallel for
  // cyklus pres steny ve smeru j
  for (int j=0; j<w.N(); j++) {
    for (int i=0; i<w.M()+1; i++) {

      Face f = g.faceJ(i, j);
      Vector2d rL(g.center(i-1, j), f.center);
      Vector2d rR(g.center(i, j), f.center);
      
      var wl = w[i-1][j] + psi[i-1][j]
	     * (gradW[i-1][j].x * rL.x + gradW[i-1][j].y * rL.y);
      
      var wr = w[i][j] + psi[i][j]
	     * (gradW[i][j].x * rR.x + gradW[i][j].y * rR.y);

      var flx = var::flux(wl, wr, f.s);

      rez[i-1][j] += flx;
      rez[i][j] -= flx;
    }
  }

#pragma omp parallel for
  // vydeleni objemem bunky a vynasobeni -1
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N(); j++) {
      rez[i][j] = -1. * rez[i][j] / g.volume(i, j);
    }
  }
}

#endif
