#ifndef SETNODEFIELD_HPP
#define SETNODEFIELD_HPP

#include <omp.h>
#include "../fvm/cellField.hpp"
#include "../fvm/nodeField.hpp"
#include "../fvm/grid.hpp"

template <typename var>
void setNodeField(const CellField<var>& w, const Grid& g, NodeField<var>& wNode) {

#pragma omp parallel for
  for (int i=0; i<g.Mnd(); i++) {
    for (int j=0; j<g.Nnd(); j++) {
      vector<double> alpha = g.alpha(i, j);
      vector<var> neighbours(4);

      neighbours[0] = w[i-1][j];
      neighbours[1] = w[i][j];
      neighbours[2] = w[i-1][j-1];
      neighbours[3] = w[i][j-1];

      var& wNd = wNode[i][j];
      wNd.zero();
      for (int k=0; k<neighbours.size(); k++) {
	wNd += alpha[k] * neighbours[k];
      }
    }
  }

}

#endif
