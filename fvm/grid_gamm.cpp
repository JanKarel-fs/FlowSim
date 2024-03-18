#include "grid_gamm.hpp"

Grid_gamm::Grid_gamm(int M, int N, int ght) {
  Mvolumes = M;
  Nvolumes = N;
  Mnodes = M + 1;
  Nnodes = N + 1;
  ghost = ght;

  nodes.allocate(-ghost, Mnodes+ghost, -ghost, Nnodes+ghost);
  facesI.allocate(-ghost, Mvolumes+ghost, -ghost, Nnodes+ghost);
  facesJ.allocate(-ghost, Mnodes+ghost, -ghost, Nvolumes+ghost);
  centers.allocate(-ghost, Mvolumes+ghost, -ghost, Nvolumes+ghost);
  volumes.allocate(-ghost, Mvolumes+ghost, -ghost, Nvolumes+ghost);


  // generovani souradnic uzlu site (vypocetni oblast)
  double dx = 3. / Mvolumes;
  for (int i=0; i<Mnodes; i++) {
    double y0 = 0.;
    double y1 = 1.;

    double x = i * dx;
    if (x>=1 && x<=2) y0 = sqrt(1.3*1.3 - pow(x-1.5, 2)) - 1.2;

    double dy = (y1 - y0) / Nvolumes;

    for (int j=0; j<Nnodes; j++) {
      double y = y0 + j * dy;
      nodes[i][j].vertex = Point2d(x, y);
    }
  }

  // pomocne vrcholy ve smeru i
  for (int i=0; i<Mnodes; i++) {
    // dolni hranice
    Vector2d s(vertex(i, 1), vertex(i, 0));

    for (int k=1; k<=ghost; k++) {
      nodes[i][-k].vertex = vertex(i, 0) + k*s;
    }

    // horni hranice
    s = Vector2d(vertex(i, Nnodes-2), vertex(i, Nnodes-1));
    for (int k=1; k<=ghost; k++) {
      nodes[i][Nnodes-1 + k].vertex = vertex(i, Nnodes-1) + k*s;
    }
  }

  // pomoce vrcholy ve smeru j
  for (int j=-ghost; j<Nnodes+ghost; j++) {
    // leva hranice
    Vector2d s(vertex(1, j), vertex(0, j));
    for (int k=1; k<=ghost; k++) {
      nodes[-k][j].vertex = vertex(0, j) + k*s;
    }

    //prava hranice
    s = Vector2d(vertex(Mnodes-2, j), vertex(Mnodes-1, j));
    for (int k=1; k<=ghost; k++) {
      nodes[Mnodes-1 + k][j].vertex = vertex(Mnodes-1, j) + k*s;
    }
  }

  // stredy bunek
  for (int i=-ghost; i<Mvolumes+ghost; i++) {
    for (int j=-ghost; j<Nvolumes+ghost; j++) {
      centers[i][j] = (vertex(i,j) + vertex(i+1, j)
		       + vertex(i+1, j+1) + vertex(i, j+1)) / 4.;
    }
  }

  // objemy bunek
  for (int i=-ghost; i<Mvolumes+ghost; i++) {
    for (int j=-ghost; j<Nvolumes+ghost; j++) {
      Vector2d e(vertex(i, j), vertex(i+1, j+1));
      Vector2d f(vertex(i+1, j), vertex(i, j+1));

      volumes[i][j] = fabs(cross(e, f)) / 2.;
    }
  }

  update();

  // nastaveni jmen sten
  for (int i=-ghost; i<Mvolumes+ghost; i++) {
    for (int j=-ghost; j<Nnodes+ghost; j++) {
      facesI[i][j].name = "internal";

      if (j<=0 || j>=Nnodes-1) facesI[i][j].name = "Wall";
    }
  }

  for (int i=-ghost; i<Mnodes+ghost; i++) {
    for (int j=-ghost; j<Nvolumes+ghost; j++) {
      facesJ[i][j].name = "internal";

      if (i<=0) facesJ[i][j].name = "Inlet";
      if (i>=Mnodes-1) facesJ[i][j].name = "Outlet";
    }
  }
}
