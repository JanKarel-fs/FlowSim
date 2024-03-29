#ifndef SETTING_HPP
#define SETTING_HPP

#include <string>
#include <set>
#include <map>
#include <cstdlib>
#include "../geometry/vector.hpp"
#include "loadDataFile.hpp"
#include "findSection.hpp"

using namespace std;

class Setting {
public:
  int grid_type;
  int mCells;
  int nCells;
  int ghostCells;
  double rhoInit;
  double pInit;
  Vector2d uInit;
  int numOfBoundaries;
  map<string, string> usedBC;
  int flux;
  double p0;
  double rho0;
  double alpha;
  double kappa;
  double CFL;
  double M2is;
  int stop;
  int spatialOrder;

  Setting(const string& fileName);
  ~Setting() {};
};

#endif
