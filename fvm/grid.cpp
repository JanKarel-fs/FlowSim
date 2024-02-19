#include "grid.hpp"

void Face::updateI(Grid& g, int i, int j) {
  Point2d a = g.vertex(i, j);
  Point2d b = g.vertex(i+1, j);

  center = (a + b) / 2.;
  s = Vector2d(b.y-a.y, a.x-b.x);
}

void Face::updateJ(Grid& g, int i, int j) {
  Point2d a = g.vertex(i, j);
  Point2d b = g.vertex(i, j+1);

  center = (a + b) / 2.;
  s = Vector2d(b.y-a.y, a.x-b.x);
}

double Gird::x(const int& i, const int& j) const {
  return nodes[i][j].vertex.x;
}

double Grid::y(const int& i, const int& j) const {
  return nodes[i][j].vertex.y;
}

Point2d Grid::vertex(const int& i, const int& j) const {
  return nodes[i][j].vertex;
}

Node Grid::node(const int& i, const int& j) const {
  return nodes[i][j];
}

Face Grid::faceI(const int& i, const int& j) const {
  return facesI[i][j];
}

Face Grid::faceJ(const int& i, const int& j) const {
  return facesJ[i][j];
}

double Grid::volume(const int& i, const int& j) const {
  return volumes[i][j];
}

Point2d Grid::center(const int& i, const int& j) const {
  return centers[i][j];
}

void Grid::update() {
  for (int i=-ghost; i<Mvolumes+ghost; i++) {
    for (int j=-ghost; j<Nnodes+ghost; j++) {
      facesI[i][j].updateI(*this, i, j);
    }
  }

  for (int i=-ghost; i<Mnodes+ghost; i++) {
    for (int j=-ghost; j<Nvolumes+ghost; j++) {
      facesJ[i][j].udateJ(*this, i, j);
    }
  }
}
