#include <iostream>
#include "fvm/grid.hpp"
#include "fvm/grid_gamm.hpp"
#include "fvm/cellField.hpp"
#include "fvm/computeRez.hpp"
#include "sources/initialisation.hpp"
#include "sources/timeStep.hpp"
#include "sources/setBoundaries.hpp"
#include "sources/setting.hpp"
#include "sources/BC.hpp"
#include "saving/saveNormResidual.hpp"
#include "saving/saveResults.hpp"
#include "compressible.hpp"

using namespace std;

int main() {
  cout << "Welcom in FlowSim!" << endl;

  Grid g = Grid_gamm(150, 50, 1);

  Setting setting("starter.txt");

  map<string, bCondition> BC;
  for (auto it=setting.usedBC.begin(); it!=setting.usedBC.end(); it++) {
    auto bCond = bcList.find(it->second);
    if (bCond != bcList.end()) {
      BC[it->first] = bcList[it->second];
    }
    else {
      cout << "There is no a such boundary condition " << it->second << "!" << endl;
      cout << "Possibilities are:" << endl;
      for (bCond=bcList.begin(); bCond!=bcList.end(); bCond++) {
	cout << bCond->first << endl;
      }
      exit(0);
    }
  }

  CellField<Compressible> w(g);
  CellField<Compressible> wStar(g);
  CellField<Compressible> rez(g);

  initialisation(w, setting);

  for (int i=0; i<setting.stop; i++) {
    double dt = timeStep(w, g, setting);

    wStar = w;
    for (int k=0; k<setting.alphaRK.size(); k++) {
      setBoundaries(wStar, g, setting, BC);
      computeRez(wStar, rez, g);

      wStar = w + dt * setting.alphaRK[k] * rez;
    }
    w = wStar;

    if (i%10 == 0) {
      saveNormResidual(rez, g, i);
      cout << "iter: " << i << ", dt = " << dt << endl;
    }
  }

  setBoundaries(w, g, setting, BC);
  saveResults(w, g);

  cout << "Bey bye!" << endl;

  return 0;
}
