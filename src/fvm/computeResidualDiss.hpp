#ifndef COMPUTERESIDUALDISS_HPP
#define COMPUTERESIDUALDISS_HPP

#include <omp.h>
#include "grid.hpp"
#include "cellField.hpp"
#include "nodeField.hpp"
#include "firstDerivatives.hpp"
#include "../primitiveVars.hpp"
#include "../sources/setNodeField.hpp"

template <typename var>
void computeResidualDiss(const CellField<var>& w, CellField<var>& rez, const Grid& g) {

  CellField<PrimitiveVars> pVars(g);
  NodeField<PrimitiveVars> pVarsNode(g);

#pragma omp parallel for
  for (int i=w.Imin(); i<w.Imax(); i++) {
    for (int j=w.Jmin(); j<w.Jmax(); j++) {
      pVars[i][j] = PrimitiveVars::set(w[i][j]);
    }
  }

  setNodeField(pVars, g, pVarsNode);

  // vypocet toku stenami ve smeru i
#pragma omp parallel for
  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N()+1; j++) {
      const Face& f = g.faceI(i, j);

      Point2d L = g.center(i, j);
      Point2d R = g.center(i, j-1);

      const PrimitiveVars& pVarsL = pVars[i][j];
      const PrimitiveVars& pVarsR = pVars[i][j-1];
      const PrimitiveVars& pVarsA = pVarsNode[i][j];
      const PrimitiveVars& pVarsB = pVarsNode[i+1][j];

      Vector2<PrimitiveVars> gradPvars;

      firstDerivatives(pVarsL, pVarsR, pVarsA, pVarsB, L, R, f.s, gradPvars);

      var wFace = 0.5 * (w[i][j] + w[i][j-1]);

      var flx = var::fluxDissipative(wFace, gradPvars, f.s);

      rez[i][j] += flx;
      rez[i][j-1] -= flx;
    }
  }

  // vypocet toku stenami ve smeru j
#pragma omp parallel for
  for (int j=0; j<w.N(); j++) {
    for (int i=0; i<w.M()+1; i++) {
      const Face& f = g.faceJ(i, j);
     

      Point2d L = g.center(i-1, j);
      Point2d R = g.center(i, j);

      const PrimitiveVars& pVarsL = pVars[i-1][j];
      const PrimitiveVars& pVarsR = pVars[i][j];
      const PrimitiveVars& pVarsA = pVarsNode[i][j];
      const PrimitiveVars& pVarsB = pVarsNode[i][j+1];

      Vector2<PrimitiveVars> gradPvars;

      firstDerivatives(pVarsL, pVarsR, pVarsA, pVarsB, L, R, f.s, gradPvars);

      var wFace = 0.5 * (w[i-1][j] + w[i][j]);

      var flx = var::fluxDissipative(wFace, gradPvars, f.s);

      rez[i-1][j] += flx;
      rez[i][j] -= flx;
    }
  }  
}

#endif
