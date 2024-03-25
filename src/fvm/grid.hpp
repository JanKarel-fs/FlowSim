#ifndef GRID_HPP
#define GRID_HPP

#include <string>
#include "../geometry/point.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/field.hpp"

using namespace std;

class Grid;

class Node {
public:
  Point2d vertex;
};

class Face {
public:
  Point2d center;
  Vector2d s;    // normalovy vektor prenasobeny velikosti steny
  string name;

  void updateI(Grid& g, int i, int j);
  void updateJ(Grid& g, int i, int j);
};

class Grid {
protected:
  int Mvolumes;   // pocet bunek ve smeru i
  int Nvolumes;   // pocet bunek ve smeru j
  int Mnodes;     // pocet vrcholu ve smeru i
  int Nnodes;     // pocet vrcholu ve smeru j
  int ghost;      // pocet vrstev pomocnych bunek

  Field2<Node> nodes;   // seznam vrcholu
  Field2<Face> facesI;  // seznam sten ve smeru i
  Field2<Face> facesJ;  // seznam sten ve smeru j
  Field2<Point2d> centers; // seznam stredu bunek
  Field2<double> volumes;  // seznam objemu bunek

public:
  int Mnd() const {return Mnodes;}
  int Nnd() const {return Nnodes;}
  int Mvol() const {return Mvolumes;}
  int Nvol() const {return Nvolumes;}
  int gh() const {return ghost;}

  double x(const int& i, const int& j) const; // x-ova souradnice vrcholu ij
  double y(const int& i, const int& j) const; // y-ova souradnice vrcholu ij
  Point2d vertex(const int& i, const int& j) const;  // souradnice vrcholu ij
  Node node(const int& i, const int& j) const; // vrchol ij
  Face faceI(const int& i, const int& j) const;
  Face faceJ(const int& i, const int& j) const;
  double volume(const int& i, const int& j) const;  // objem bunky ij
  Point2d center(const int& i, const int& j) const; // stred bunky ij

  void update();
};

#endif
