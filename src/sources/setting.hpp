#ifndef SETTING_HPP
#define SETTING_HPP

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
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
  string name1;
  string name2;
  double rhoInit;
  double pInit;
  Vector2d uInit;
  int numOfBoundaries;
  map<string, string> usedBC;
  string flux;
  double p0;
  double rho0;
  double alpha;
  double kappa;
  double R;
  double Pr;
  double CFL;
  double M2is;
  int stop;
  int spatialOrder;
  int temporalOrder;
  vector<double> alphaRK;
  int limiter;
  int solver;
  int convection;
  int diffusion;
  string nodeWeightType;

  Setting(const string& fileName);
  ~Setting() {};
};

#endif
