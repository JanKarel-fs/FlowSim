#ifndef BARTHJESPERSEN_HPP
#define BARTHJESPERSEN_HPP

#include <vector>
#include <omp.h>
#include "cellField.hpp"
#include "grid.hpp"

using namespace std;

template <typename var>
void barthJespersen(const CellField<var>& w, const CellField<Vector2<var> >& gradW,
		    CellField<var>& psi, const Grid& g) {

#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      psi[i][j].zero();
    }
  }

#pragma omp parallel for
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N(); j++) {

      psi[i][j].ones();
      
      vector<Point2d> centers(4);
      vector<var> wK(4);

      centers[0] = g.center(i-1, j);  centers[1] = g.center(i+1, j);
      centers[2] = g.center(i, j-1);  centers[3] = g.center(i, j+1);

      wK[0] = w[i-1][j];  wK[1] = w[i+1][j];
      wK[2] = w[i][j-1];  wK[3] = w[i][j+1];

      const var& wij = w[i][j];
      Point2d center_ij = g.center(i, j);

      var wMin = wij;
      var wMax = wij;

      for (int k=0; k<4; k++) {
	wMin = var::min(wMin, wK[k]);
	wMax = var::max(wMax, wK[k]);
      }

      for (int m=0; m<var::nVars; m++) {
	for (int k=0; k<4; k++) {
	  double psiM = 1.;

	  Vector2d r(center_ij, centers[k]);
	  double delta2 = gradW[i][j].x[m] * r.x + gradW[i][j].y[m] * r.y;

	  if (delta2 > 0.) {
	    psiM = min(1., (wMax[m] - wij[m])/delta2);
	  }
	  else if (delta2 < 0.) {
	    psiM = min(1., (wMin[m] - wij[m])/delta2);
	  }

	  psi[i][j][m] = min(psi[i][j][m], psiM);
	}
      }
    }
  }
}

#endif
