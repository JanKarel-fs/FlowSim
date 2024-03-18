#ifndef BC_HPP
#define BC_HPP

#include <map>
#include "../sources/typedefs.hpp"
#include "../sources/inlet.hpp"
#include "../sources/outlet.hpp"
#include "../sources/wall.hpp"

using namespace std;

map<string, bCondition> bcList{condition("inlet", inlet),
			       condition("outlet", outlet),
			       condition("wall", wall)};

#endif
