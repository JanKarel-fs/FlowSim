#include "initialisation.hpp"

void initialisation(CellField<Compressible>& w, const Setting& setting) {

  system("mkdir -p results");  // vytvoreni slozky results - Linux
  // system("md -p results");  // vytvoreni slozky results - Windows
  system("rm -f results/*");   // smazani obsahu slozky results - Linux
  // system("del -f results\*"); // Windows

  switch (setting.solver) {
  case 1: {
    step<Compressible> = stepExplicit<Compressible>;
    switch (setting.flux) {
    case 1:
      Compressible::flux = Compressible::Upwind;
      break;
    case 2:
      Compressible::flux = Compressible::Rusanov;
      break;
    default:
      cout << "No a such possibility for a flux splitter!" << endl;
      cout << "Possibilities are: 1 - Upwind, 2 - Rusanov" << endl;
      exit(10);
    }
  }
    break;

  case 2: {
    step<Compressible> = stepImplicit<Compressible>;
    switch (setting.flux) {
    case 1:
      Compressible::fluxImplicit = Compressible::UpwindImplicit;
      break;
    case 2:
      Compressible::fluxImplicit = Compressible::RusanovImplicit;
      break;
    default:
      cout << "No a such possibility for a flux splitter!" << endl;
      cout << "Possibilities: 1 - Upwind, 2 - Rusanov" << endl;
      exit(1);
    }

    switch (setting.temporalOrder) {
    case 1: timeIncrement<Compressible> = timeIncrementFirstOrder<Compressible>;
      break;
    case 2:  timeIncrement<Compressible> = timeIncrementSecondOrder<Compressible>;
      break;
    default:
      cout << "No such temporal order!" << endl;
      exit(0);
    }
  }
    break;

  default:
    cout << "There is no possible choice for a solver!" << endl;
    cout << "Possibilities are 1 for explicit solver, 2 for implicit solver." << endl;
    exit(1);
  }

  switch(setting.spatialOrder) {
  case 1:
    grad<Compressible> = zeroGrad<Compressible>;
    limiter<Compressible> = zeroLimiter<Compressible>;
    break;
  case 2:
    grad<Compressible> = gradLSM<Compressible>;
    switch(setting.limiter) {
    case 1:
      limiter<Compressible> = barthJespersen<Compressible>;
      break;
    case 2:
      limiter<Compressible> = venkatakrishnan<Compressible>;
      break;
    default:
      cout << "No a such possibility for a limiter!" << endl;
      cout << "Possibilities are: 1 - BarthJespersen, 2 - Venkatakrishnan" << endl;
      exit(12);
    }
    break;
  default:
    cout << "No a such possibility for a spatial order!" << endl;
    cout << "Possibilities are: 1 - 1st order, 2 - 2nd order" << endl;
    exit(11);
  }

  Compressible::kappa = setting.kappa;
  Compressible::R = setting.R;
  Compressible::Pr = setting.Pr;
  Compressible::cv = Compressible::R / (Compressible::kappa - 1.);
  Compressible::cp = Compressible::cv * Compressible::kappa;

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
