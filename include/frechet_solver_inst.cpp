// file frechet_solver_inst.cpp

#include "instantiate.h"
#include "frechet_solver.cpp"

// Explicitly instantiate Frechet_solver; traits_t is a typedef for a
// concrete type, defined in instantiate.h 

template class Frechet_solver< traits_t >;


