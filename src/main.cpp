#include <iostream>
#include "fvm/grid.hpp"
#include "fvm/cellField.hpp"
#include "sources/initialisation.hpp"
#include "sources/timeStep.hpp"
#include "sources/setBoundaries.hpp"
#include "sources/setting.hpp"
#include "sources/BC.hpp"
#include "saving/saveNormResidual.hpp"
#include "saving/saveResults.hpp"
#include "sources/setGrid.hpp"
#include "sources/step.hpp"
#include "sources/linearSolver.hpp"
#include "compressible.hpp"

using namespace std;

int main(int argc,char **args) {

  PetscInitialize( &argc , &args , (char *)0 , 0 );
  
  cout << "Welcom in FlowSim!" << endl;

  Setting setting("starter.txt");
  
  Grid g;
  setGrid(g, setting);

  LinearSolver<Compressible> linSolver(setting.solver, g);
  
  map<string, bcWithJacobian> BC;
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
  CellField<Compressible> wOld(g);
  CellField<Compressible> rez(g);

  initialisation(w, setting);
  wOld = w;

  for (int i=0; i<setting.stop; i++) {
    double dt = timeStep(w, g, setting);

    step<Compressible>(w, wOld, rez, g, dt, BC, linSolver, setting);
    
    if (i%10 == 0) {
      saveNormResidual(rez, g, i);
      cout << "iter: " << i << ", dt = " << dt << endl;
    }
  }

  setBoundaries(w, g, setting, BC);
  saveResults(w, g);

  cout << "Bey bye!" << endl;

  linSolver.free();

  PetscFinalize();

  return 0;
}
