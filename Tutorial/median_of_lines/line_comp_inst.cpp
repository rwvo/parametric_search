// file line_comp_inst.cpp

#include "instantiate.h"
#include "line_comp.cpp"

// Explicitly instantiate Line_comp; traits_t is a typedef
// for a concrete type, defined in instantiate.h

template class Line_comp< traits_t >;

