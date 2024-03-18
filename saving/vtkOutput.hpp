#ifndef VTKOUTPUT_HPP
#define VTKOUTPUT_HPP

#include <fstream>
#include <iomanip>
#include <string>
#include "../fvm/grid.hpp"
#include "../fvm/nodeField.hpp"

using namespace std;

class vtkOutput {
  ofstream fout;

public:
  vtkOutput(const string& name);
  ~vtkOutput();

  void save(const Grid& g);
  void save(const NodeField<double>& p, const string& name);
  void save(const NodeField<Vector2d>& d, const string& name);
};
 
#endif
