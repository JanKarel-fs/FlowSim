#include "timeStep.hpp"

double timeStep(const CellField<Compressible>& w, const Grid& g,
		Setting& setting) {
  double dt = 1e20;

  int M = g.Mvol();
  int N = g.Nvol();

#pragma omp parallel for reduction(min:dt)
  for (int i=0; i<M; i++){
    for (int j=0; j<N; j++) {
      const Compressible& wij = w[i][j];
      Vector2d U = wij.rhoU / wij.rho;
      
      Point2d A = (g.vertex(i, j) + g.vertex(i, j+1)) / 2.;
      Point2d B = (g.vertex(i+1, j) + g.vertex(i+1, j+1)) / 2.;
      Point2d C = (g.vertex(i, j) + g.vertex(i+1, j)) / 2.;
      Point2d D = (g.vertex(i, j+1) + g.vertex(i+1, j+1)) / 2.;

      Vector2d s1(A, B);
      Vector2d s2(C, D);

      double dX = s1.length();
      double dY = s2.length();

      double lambda = 0.;

      switch (setting.convection) {
      case 0: break;
      case 1: {
	double a = wij.a();	
	double uTilde = dot(U, s1) / dX;
	double vTilde = dot(U, s2) / dY;
	double lambdaConv = (fabs(uTilde) + a) / dX + (fabs(vTilde) + a) / dY;
	lambda += lambdaConv;
      }
	break;
      default:
	cout << "No such possibility for a computation of a convective part of a time step!" << endl;
	exit(10);
      }
      
      switch (setting.diffusion) {
      case 0:break;
      case 1: {
	double mu = wij.mu();
	double rho = wij.rho;
	double lambdaDiff = 2. * mu/rho * (1. / (dX*dX) + 1. / (dY*dY));
	lambda += lambdaDiff;
      }
	break;
      default:
	cout << "No such possibility for a computation of a dissipative part of a time step!" << endl;
	exit(10);
      }

      double dtij = setting.CFL / lambda;

      dt = min(dt, dtij);
    }
  }

  setting.updateCFL();

  return dt;
}
