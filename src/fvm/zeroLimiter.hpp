#ifndef ZEROLIMITER_HPP
#define ZEROLIMITER_HPP

#include <omp.h>
#include "cellField.hpp"
#include "grid.hpp"

template <typename var>
void zeroLimiter(const CellField<var>& w, const CellField<Vector2<var> >& gradW,
		 CellField<var>& psi, const Grid& g) {

#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      psi[i][j].zero();
    }
  }

}

#endif
