#include "setting.hpp"

Setting::Setting(const string& fileName) {
  set<string> sections;
  sections.insert("GRID");  sections.insert("INITIAL_CONDITIONS");
  sections.insert("BOUNDARY_CONDITIONS");  sections.insert("FLUX_SPLITTER");
  sections.insert("TIME");  sections.insert("SYSTEM");
  sections.insert("PHYSICAL_VALUES");
  sections.insert("SAVING");  sections.insert("ACCURACY");

  map<string, vector<string> > dataFile;

  loadDataFile(fileName, sections, dataFile);

  // nacitani informaci o siti
  mCells = 0;
  nCells = 0;
  string section = "GRID";
  findSection(dataFile, "grid_type", section, grid_type);
  switch (grid_type) {
  case 1:
    findSection(dataFile, "mCells", section, mCells);
    findSection(dataFile, "nCells", section, nCells);
    break;
  case 2:
    findSection(dataFile, "name1", section, name1);
    findSection(dataFile, "name2", section, name2);
    break;
  default:
    cout << "There is no a such possibility for a grid_type, possibilities are: 1 for grid type GAMM, 2 for a grid type XY!" << endl;
    exit(1);
  }
  findSection(dataFile, "ghostCells", section, ghostCells);

  // nacitani informaci o pocatecnich podminkach
  section = "INITIAL_CONDITIONS";
  findSection(dataFile, "rhoInit", section, rhoInit);
  findSection(dataFile, "pInit", section, pInit);
  double x, y;
  findSection(dataFile, "uInit", section, x);
  findSection(dataFile, "vInit", section, y);
  uInit = Vector2d(x, y);

  // nacitani informaci o okrajovych podminkach
  section = "BOUNDARY_CONDITIONS";
  findSection(dataFile, "numOfBoundaries", section, numOfBoundaries);
  for (int i=1; i<=numOfBoundaries; i++) {
    string part;
    stringstream streamValue;

    streamValue << i;
    streamValue >> part;

    string boundary = "boundary" + part;
    string bcType = "bcType" + part;

    string boundaryValue, bcTypeValue;
    findSection(dataFile, boundary, section, boundaryValue);
    findSection(dataFile, bcType, section, bcTypeValue);

    usedBC[boundaryValue] = bcTypeValue;
  }
  findSection(dataFile, "alpha", section, alpha);
  findSection(dataFile, "M2is", section, M2is);

  // nacitani informaci o numerickem toku
  section = "FLUX_SPLITTER";
  findSection(dataFile, "flux", section, flux);

  // nacitani informaci o numerickem toku
  section = "ACCURACY";
  findSection(dataFile, "spatialOrder", section, spatialOrder);
  findSection(dataFile, "temporalOrder", section, temporalOrder);
  switch (temporalOrder) {
  case 1:
    alphaRK.resize(1);
    alphaRK[0] = 1.;
    break;
  case 2:
    alphaRK.resize(3);
    alphaRK[0] = 0.5;  alphaRK[1] = 0.5;  alphaRK[2] = 1.;
    break;
  default:
    cout << "No a such possibility for a temporal order!" << endl;
    cout << "Possibilities are: 1 - 1st order, 2 - 2nd order" << endl;
    exit(11);
  }
  findSection(dataFile, "limiter", section, limiter);

  // nacitani informaci o case
  section = "TIME";
  findSection(dataFile, "CFL", section, CFL);

  // nacitani informaci o systemu
  section = "SYSTEM";
  findSection(dataFile, "convection", section, convection);
  findSection(dataFile, "diffusion", section, diffusion);
  findSection(dataFile, "nodeWeightType", section, nodeWeightType);

  // nacitani informaci o fyzikalnich promennych
  section = "PHYSICAL_VALUES";
  findSection(dataFile, "kappa", section, kappa);
  findSection(dataFile, "R", section, R);
  findSection(dataFile, "Pr", section, Pr);
  findSection(dataFile, "rho0", section, rho0);
  findSection(dataFile, "p0", section, p0);

  // nacitani informaci o ukladani
  section = "SAVING";
  findSection(dataFile, "stop", section, stop);
}
