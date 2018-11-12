// file frechet_comp_inst.cpp

#include "instantiate.h"
#include "frechet_comp.cpp"

// Explicitly instantiate Frechet_comp; traits_t is a typedef for a
// concrete type, defined in instantiate.h

template class Frechet_comp< traits_t >;


