#ifndef SETGRID_HPP
#define SETGRID_HPP

#include <iostream>
#include <cstdlib>
#include "../fvm/grid.hpp"
#include "../fvm/grid_gamm.hpp"
#include "../fvm/grid_xy.hpp"
#include "setting.hpp"

using namespace std;

void setGrid(Grid& g, const Setting& setting);

#endif
