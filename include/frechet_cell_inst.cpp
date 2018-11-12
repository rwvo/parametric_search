// file frechet_cell_inst.cpp

#include "instantiate.h"
#include "frechet_cell.cpp"

// Explicitly instantiate Frechet_cell_edge and Frechet_cell; traits_t
// is a typedef for a concrete type, defined in instantiate.h

template class Frechet_cell_edge< traits_t >;
template class Frechet_cell< traits_t >;

