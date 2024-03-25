#ifndef GRAD_HPP
#define GRAD_HPP

#include "cellField.hpp"
#include "grid.hpp"

template <typename var>
void (*grad)(const CellField<var>& w, CellField<Vector2<var> >& gradW,
	     const Grid& g);

#endif
