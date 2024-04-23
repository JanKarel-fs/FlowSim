#ifndef BC_HPP
#define BC_HPP

#include <map>
#include "../sources/typedefs.hpp"
#include "../sources/inlet.hpp"
#include "../sources/outlet.hpp"
#include "../sources/slipWall.hpp"
#include "noSlipWall.hpp"
#include "symmetry.hpp"
#include "homogeneousNeumann.hpp"
#include "inletJacobian.hpp"
#include "outletJacobian.hpp"
#include "slipWallJacobian.hpp"
#include "noSlipWallJacobian.hpp"
#include "symmetryJacobian.hpp"
#include "homogeneousNeumannJacobian.hpp"

using namespace std;

map<string, bcWithJacobian> bcList =
    {condition("inlet", bcWithJacobian(inlet, inletJacobian)),
     condition("outlet", bcWithJacobian(outlet, outletJacobian)),
     condition("slipWall", bcWithJacobian(slipWall, slipWallJacobian)),
     condition("noSlipWall", bcWithJacobian(noSlipWall, noSlipWallJacobian)),
     condition("symmetry", bcWithJacobian(symmetry, symmetryJacobian)),
     condition("homogeneousNeumann", bcWithJacobian(homogeneousNeumann,
						    homogeneousNeumannJacobian))};

#endif
