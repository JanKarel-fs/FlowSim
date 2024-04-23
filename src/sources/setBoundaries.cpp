#include "setBoundaries.hpp"

void setBoundaries(CellField<Compressible>& w, const Grid& g,
		   const Setting& setting, const map<string, bcWithJacobian>& BC) {
  int M = w.M();
  int N = w.N();
  int gh = w.gh();
  
  // nasteveni hodnot v levych a pravych pomocnych bunkach
  for (int j=0; j<w.N(); j++) {
    // leva hranice (vstup)
    Compressible wInside = w[0][j];
    Face f = g.faceJ(0, j);

    auto it = BC.find(f.name);
    // Chybi otestovani, jestli jsme nasli danou okrajovou podminku
    Compressible wOut = it->second.first(wInside, f.s, setting);

    for (int k=1; k<=gh; k++) {
      w[-k][j] = wOut;
    }

    // prava hranice (vystup)
    wInside = w[M-1][j];
    f = g.faceJ(M, j);

    it = BC.find(f.name);
    wOut = it->second.first(wInside, f.s, setting);

    for (int k=1; k<=gh; k++) {
      w[M-1+k][j] = wOut;
    }
  }

  // nastaveni hodnot v dolnich a hornich pomocnych bunkach
  for (int i=-gh; i<M+gh; i++) {
    // dolni hranice
    Compressible wInside = w[i][0];
    Face f = g.faceI(i, 0);

    auto it = BC.find(f.name);
    Compressible wOut = it->second.first(wInside, f.s, setting);

    for (int k=1; k<=gh; k++) {
      w[i][-k] = wOut;
    }

    // horni hranice
    wInside = w[i][N-1];
    f = g.faceI(i, N);
    
    it = BC.find(f.name);
    wOut = it->second.first(wInside, f.s, setting);

    for (int k=1; k<=gh; k++) {
      w[i][N-1+k] = wOut;
    }
  }
}
