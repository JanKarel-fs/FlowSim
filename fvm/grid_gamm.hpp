#ifndef GRID_GAMM_HPP
#define GRID_GAMM_HPP

#include <cmath>
#include "grid.hpp"

class Grid_gamm : public Grid {
public:
  Grid_gamm() {};
  Grid_gamm(int M, int N, int ght);
  ~Grid_gamm() {};
};

#endif
