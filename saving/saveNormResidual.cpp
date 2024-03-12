#include "saveNormResidual.hpp"

void saveNormResidual(const CellField<Compressible>& rez,
		      const Grid& g, const int& iter) {

  Compressible L2;
  Compressible Linf;

  L2.zero();
  Linf.zero();

  for (int i=0; i<rez.M(); i++) {
    for (int j=0; j<rez.N(); j++) {
      const Compressible& R = rez[i][j];

      L2 += R*R * g.volume(i, j);
      Linf = Compressible::max(Linf, Compressible::fabs(R));
    }
  }

  L2 = Compressible::sqrt(L2);

  ofstream fout("results/L2.txt", ios::app);
  fout << iter << " " << L2.rho << " " << L2.rhoU.x << " "
       << L2.rhoU.y << " " << L2.e << std::endl;

  fout.close();

  fout.open("results/Linf.txt", ios::app);
  fout << iter << " " << Linf.rho << " " << Linf.rhoU.x << " "
       << Linf.rhoU.y << " " << Linf.e << std::endl;

  fout.close();
}
