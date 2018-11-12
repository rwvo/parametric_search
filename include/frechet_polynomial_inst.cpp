// file frechet_polynomial_inst.cpp

#include "instantiate.h"
#include "frechet_polynomial.cpp"


// Explicitly instantiate Frechet_polynomial; traits_t is a typedef
// for a concrete type, defined in instantiate.h

template class Frechet_polynomial< traits_t >;

//-------------------------------------------------- 

