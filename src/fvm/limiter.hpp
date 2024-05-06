#ifndef LIMITER_HPP
#define LIMITER_HPP

#include "cellField.hpp"
#include "grid.hpp"

template <typename var>
void (*limiter)(const CellField<var>& w, const CellField<Vector2<var> >& gradW,
		CellField<var>& psi, const Grid& g);

#endif
