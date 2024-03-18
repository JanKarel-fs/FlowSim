#include "initialisation.hpp"

void initialisation(CellField<Compressible>& w, const Setting& setting) {

  system("mkdir -p results");  // vytvoreni slozky results - Linux
  // system("md -p results");  // vytvoreni slozky results - Windows
  system("rm -f results/*");   // smazani obsahu slozky results - Linux
  // system("del -f results\*"); // Windows

  switch (setting.flux) {
  case 1:
    Compressible::flux = Compressible::Upwind;
    break;
  default:
    cout << "No a such possibility for a flux splitter!" << endl;
    cout << "Possibilities are: 1 - Upwind" << endl;
    exit(10);
  }

  Compressible::kappa = setting.kappa;

  const double& rhoInit = setting.rhoInit;
  const Vector2d& uInit = setting.uInit;
  const double& pInit = setting.pInit;

  double eInit = pInit / (Compressible::kappa - 1.) + 0.5 * rhoInit
               * (uInit.x*uInit.x + uInit.y*uInit.y);


  for (int i=0; i<w.M(); i++) {
    for (int j=0; j<w.N(); j++) {
      w[i][j] = Compressible(rhoInit, rhoInit*uInit, eInit);
    }
  }
}
