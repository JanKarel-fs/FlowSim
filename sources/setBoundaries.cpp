#include "setBoundaries.hpp"

void setBoundaries(CellField<Compressible>& w, const Grid& g,
		   const Setting& setting) {
  int M = w.M();
  int N = w.N();
  int gh = w.gh();
  
  // nasteveni hodnot v levych a pravych pomocnych bunkach
  for (int j=0; j<w.N(); j++) {
    // leva hranice (vstup)
    Compressible wInside = w[0][j];
    Vector2d s = g.faceJ(0, j).s;

    Compressible wOut = inlet(wInside, s, setting);

    for (int k=1; k<=gh; k++) {
      w[-k][j] = wOut;
    }

    // prava hranice (vystup)
    wInside = w[M-1][j];
    s = g.faceJ(M, j).s;

    wOut = outlet(wInside, s, setting);

    for (int k=1; k<gh; k++) {
      w[M-1+k][j] = wOut;
    }
  }

  // nastaveni hodnot v dolnich a hornich pomocnych bunkach
  for (int i=-gh; i<M+gh; i++) {
    // dolni hranice
    Compressible wInside = w[i][0];
    Vector2d s = g.faceI(i, 0);

    Compressible wOut = wall(wInside, s, setting);

    for (int k=1; k<=gh; k++) {
      w[i][-k] = wOut;
    }

    // horni hranice
    wInside = w[i][N-1];
    s = g.faceI(i, N).s;

    wOut = wall(wInside, s, setting);

    for (int k=1; k<=gh; k++) {
      w[i][N-1+k] = wOut;
    }
  }
}
