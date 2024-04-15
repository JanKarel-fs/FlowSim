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

double Grid::x(const int& i, const int& j) const {
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

vector<double> Grid::alpha(const int& i, const int& j) const {
  return nodes[i][j].alpha;
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
      facesJ[i][j].updateJ(*this, i, j);
    }
  }
}

void Grid::computeAlphaNodeWeight(Grid& g) {
  for (int i=0; i<g.Mnd(); i++) {
    for (int j=0; j<g.Nnd(); j++) {
      double volume = g.volume(i-1, j) + g.volume(i,j)
	            + g.volume(i-1, j-1) + g.volume(i, j-1);

      Node& nd = g.nodes[i][j];
      nd.alpha.resize(4);

      nd.alpha[0] = g.volume(i-1, j) / volume;
      nd.alpha[1] = g.volume(i, j) / volume;
      nd.alpha[2] = g.volume(i-1, j-1) / volume;
      nd.alpha[3] = g.volume(i, j-1) / volume;
    }
  }
}

void Grid::computeAlphaNodeLSM(Grid& g) {
  for (int i=0; i<g.Mnd(); i++) {
    for (int j=0; j<g.Nnd(); j++) {
      Point2d V = g.vertex(i, j);

      vector<Point2d> centers(4);
      centers[0] = g.center(i-1, j);
      centers[1] = g.center(i, j);
      centers[2] = g.center(i-1, j-1);
      centers[3] = g.center(i, j-1);

      double Rx = 0.;
      double Ry = 0.;
      double Ixx = 0.;
      double Iyy = 0.;
      double Ixy = 0.;

      for (int k=0; k<centers.size(); k++) {
	double Jx = centers[k].x - V.x;
	double Jy = centers[k].y - V.y;

	Rx += Jx;
	Ry += Jy;
	Ixx += Jx * Jx;
	Iyy += Jy * Jy;
	Ixy += Jx * Jy;
      }

      double D = Ixx * Iyy - Ixy * Ixy;

      double lx = (Ry*Ixy - Rx*Iyy) / D;
      double ly = (Rx*Ixy - Ry*Ixx) / D;

      Node& nd = g.nodes[i][j];
      int N = centers.size();

      nd.alpha.resize(N);
      for (int k=0; k<centers.size(); k++) {
	nd.alpha[k] = (1. + lx*(centers[k].x - V.x) + ly*(centers[k].y - V.y))
	            / (N + lx*Rx + ly*Ry);
      }
    }
  }
}

void (*Grid::computeAlphaNode)(Grid& g);

map<string, coefficients> Grid::mCoefficients = {pair<string, coefficients>("Weight", computeAlphaNodeWeight),
						 pair<string, coefficients>("LSM", computeAlphaNodeLSM)};
