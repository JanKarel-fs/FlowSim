#ifndef STEP_HPP
#define STEP_HPP

#include "../fvm/grid.hpp"
#include "../fvm/cellField.hpp"
#include "../fvm/computeRez.hpp"
#include "typedefs.hpp"
#include "setting.hpp"
#include "setBoundaries.hpp"

template <typename var>
void step(CellField<var>& w, CellField<var>& rez, const Grid& g, const double& dt,
	  const map<string, bCondition>& BC, const Setting& setting) {

  CellField<var> wStar(g);

  wStar = w;
  for (int k=0; k<setting.alphaRK.size(); k++) {
    setBoundaries(wStar, g, setting, BC);

    computeRez(wStar, rez, g, setting);
      
    wStar = w + setting.alphaRK[k] * dt * rez;
  }

  w = wStar;
  
}

#endif
