#ifndef GRADLSM_HPP
#define GRADLSM_HPP

#include <vector>
#include <omp.h>
#include "cellField.hpp"
#include "grid.hpp"

using namespace std;

template <typename var>
void gradLSM(const CellField<var>& w, CellField<Vector2<var> >& gradW,
	     const Grid& g) {

#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      gradW[i][j].x.zero();
      gradW[i][j].y.zero();
    }
  }

#pragma omp parallel for
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N(); j++) {
      vector<Point2d> centers(8);
      vector<var> wK(8);

      int k = 0;
      for (int p=i-1; p<=i+1; p++) {
	for (int r=j-1; r<=j+1; r++) {
	  if (p!=i || r!=j) {
	    centers[k] = g.center(p, r);
	    wK[k] = w[p][r];
	    k++;
	  }
	}
      }

      const var& wij = w[i][j];
      Point2d center_ij = g.center(i, j);

      double Ixx = 0.;
      double Iyy = 0.;
      double Ixy = 0.;
      var Jx;
      Jx.zero();
      var Jy;
      Jy.zero();

      for (k=0; k<8; k++) {
	double Ix = centers[k].x - center_ij.x;
	double Iy = centers[k].y - center_ij.y;

	Ixx += Ix * Ix;
	Iyy += Iy * Iy;
	Ixy += Ix * Iy;

	Jx += (wK[k] - wij) * Ix;
	Jy += (wK[k] - wij) * Iy;
      }

      double D = Ixx * Iyy - Ixy * Ixy;

      gradW[i][j].x = (Jx * Iyy - Jy * Ixy) / D;
      gradW[i][j].y = (Jy * Ixx - Jx * Ixy) / D;
    }
  }
}

#endif
