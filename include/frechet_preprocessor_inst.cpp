// file frechet_preprocessor_inst.cpp

#include "instantiate.h"
#include "frechet_preprocessor.cpp"
#include "vector"

namespace ns_parametric_search {

  // Explicitly instantiate Frechet_preprocessor; traits_t is a
  // typedef for a concrete type, defined in instantiate.h
  typedef std::vector< traits_t::polynomial_t >::iterator iter_t;
  template class Frechet_preprocessor< traits_t, iter_t >;
  template class Frechet_precomp< traits_t, iter_t >;

} // namespace ns_parametric_search
