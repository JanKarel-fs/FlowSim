#include "setting.hpp"

Setting::Setting(const string& fileName) {
  set<string> sections;
  sections.insert("GRID");  sections.insert("INITIAL_CONDITIONS");
  sections.insert("BOUNDARY_CONDITIONS");  sections.insert("FLUX_SPLITTER");
  sections.insert("TIME");  sections.insert("PHYSICAL_VALUES");
  sections.insert("SAVING");

  map<string, vector<string> > dataFile;

  loadDataFile(fileName, sections, dataFile);

  // nacitani informaci o siti
  string section = "GRID";
  findSection(dataFile, "grid_type", section, grid_type);
  switch (grid_type) {
  case 1:
    findSection(dataFile, "mCells", section, mCells);
    findSection(dataFile, "nCells", section, nCells);
    break;
  default:
    cout << "There is no a such possibility for a grid_type!" << endl;
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

  // nacitani informaci o case
  section = "TIME";
  findSection(dataFile, "CFL", section, CFL);

  // nacitani informaci o fyzikalnich promennych
  section = "PHYSICAL_VALUES";
  findSection(dataFile, "kappa", section, kappa);
  findSection(dataFile, "rho0", section, rho0);
  findSection(dataFile, "p0", section, p0);

  // nacitani informaci o ukladani
  section = "SAVING";
  findSection(dataFile, "stop", section, stop);
}
