#include "timeStep.hpp"

double timeStep(const CellField<Compressible>& w, const Grid& g,
		const Setting& setting) {
  double dt = 1e20;

  int M = g.Mvol();
  int N = g.Nvol();

  for (int i=0; i<M; i++){
    for (int j=0; j<N; j++) {
      const Compressible& wij = w[i][j];
      
      Point2d A = (g.vertex(i, j) + g.vertex(i, j+1)) / 2.;
      Point2d B = (g.vertex(i+1, j) + g.vertex(i+1, j+1)) / 2.;
      Point2d C = (g.vertex(i, j) + g.vertex(i+1, j)) / 2.;
      Point2d D = (g.vertex(i, j+1) + g.vertex(i+1, j+1)) / 2.;

      Vector2d s1(A, B);
      Vector2d s2(C, D);

      double dX = s1.length();
      double dY = s2.length();

      double a = wij.a();
      Vector2d U = wij.rhoU / wij.rho;

      double uTilde = dot(U, s1) / dX;
      double vTilde = dot(U, s2) / dY;

      double lambda = (fabs(uTilde) + a) / dX + (fabs(vTilde) + a) / dY;

      double dtij = setting.CFL / lambda;

      dt = min(dt, dtij);
    }
  }

  return dt;
}
