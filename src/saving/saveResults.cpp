#include "saveResults.hpp"

void saveResults(const CellField<Compressible>& w, const Grid& g) {
  NodeField<double> rho(g);
  NodeField<Vector2d> u(g);
  NodeField<double> e(g);
  NodeField<double> p(g);
  NodeField<double> Ma(g);

  for (int i=0; i<rho.M(); i++) {
    for (int j=0; j<rho.N(); j++) {
      Compressible wNij;

      wNij.zero();

      double volume = g.volume(i, j) + g.volume(i-1, j)
	            + g.volume(i-1, j-1) + g.volume(i, j-1);

      wNij = w[i][j]*g.volume(i, j) + w[i-1][j]*g.volume(i-1, j)
	+ w[i-1][j-1]*g.volume(i-1, j-1) + w[i][j-1]*g.volume(i, j-1);

      wNij /= volume;

      rho[i][j] = wNij.rho;
      u[i][j] = wNij.rhoU / wNij.rho;
      e[i][j] = wNij.e;
      p[i][j] = wNij.p();
      Ma[i][j] = wNij.Ma();
    }
  }

  vtkOutput v("results/results.vtk");

  v.save(g);
  v.save(rho, "rho");
  v.save(u, "u");
  v.save(e, "e");
  v.save(p, "p");
  v.save(Ma, "Ma");
}
