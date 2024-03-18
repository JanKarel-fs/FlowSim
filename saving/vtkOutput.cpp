#include "vtkOutput.hpp"

vtkOutput::vtkOutput(const string& name) {
  fout.open(name.c_str());

  fout << "# vtk DataFile Version 3.1" << endl;
  fout << "Results produced by a package FlowSim" << endl;
  fout << "ASCII" << endl;
  fout << "DATASET STRUCTURED_GRID" << endl << endl;

  fout << scientific << setprecision(6);
}

vtkOutput::~vtkOutput() {
  fout.close();
}

void vtkOutput::save(const Grid& g) {
  int totalSize = g.Mnd() * g.Nnd();

  fout << "DIMENSIONS " << g.Mnd() << " " << g.Nnd() << " 1" << endl;
  fout << "POINTS " << totalSize << " DOUBLE" << endl;
  for (int j=0; j<g.Nnd(); j++) {
    for (int i=0; i<g.Mnd(); i++) {
      fout << g.vertex(i,j).x << " " << g.vertex(i,j).y << " 0.0"
	   << endl;
    }
  }

  fout << "POINT_DATA " << totalSize << endl;
}

void vtkOutput::save(const NodeField<double>& p, const string& name) {
  fout << "SCALARS " << name << "  DOUBLE" << endl;
  fout << "LOOKUP_TABLE default" << endl;

  for (int j=0; j<p.N(); j++) {
    for (int i=0; i<p.M(); i++) {
      fout << p[i][j] << endl;
    }
  }
  fout << endl;
}

void vtkOutput::save(const NodeField<Vector2d>& d, const string& name) {
  fout << "VECTORS " << name << "  DOUBLE" << endl;

  for (int j=0; j<d.N(); j++) {
    for (int i=0; i<d.M(); i++) {
      fout << d[i][j].x << " " << d[i][j].y << " 0.0" << endl;
    }
  }
  fout << endl;
}
