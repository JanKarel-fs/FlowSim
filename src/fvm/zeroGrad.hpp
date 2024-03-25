#ifndef ZEROGRAD_HPP
#define ZEROGRAD_HPP

#include <omp.h>
#include "cellField.hpp"
#include "grid.hpp"

template <typename var>
void zeroGrad(const CellField<var>& w, CellField<Vector2<var> >& gradW,
	      const Grid& g) {

#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      gradW[i][j].x.zero();
      gradW[i][j].y.zero();
    }
  }

}

#endif
