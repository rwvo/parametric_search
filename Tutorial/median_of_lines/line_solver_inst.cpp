// file line_solver_inst.cpp

#include "instantiate.h"
#include "line_solver.cpp"

// Explicitly instantiate Line_comp; traits_t is a typedef
// for a concrete type, defined in instantiate.h

template class Line_solver< traits_t >;

