#ifndef STEPEIMPLICIT_HPP
#define STEPIMPLICIT_HPP

#include "fvm/grid.hpp"
#include "fvm/cellField.hpp"
#include "fvm/computeRezImplicit.hpp"
#include "sources/typedefs.hpp"
#include "sources/setting.hpp"
#include "sources/setBoundaries.hpp"
#include "sources/linearSolver.hpp"

template <typename var>
void stepImplicit(CellField<var>& w, CellField<var>& wOld, CellField<var>& rez, const Grid& g,
		  const double& dt, const map<string, bcWithJacobian>& BC,
		  LinearSolver<var>& linSolver, const Setting& setting) {

  linSolver.reset();

  setBoundaries(w, g, setting, BC);

  computeRezImplicit(w, wOld, g, BC, dt, linSolver, setting);

  linSolver.solve();

  linSolver.getResults(rez);

  wOld = w;

  w = w + rez;

  rez = rez / dt;
  
}

#endif
