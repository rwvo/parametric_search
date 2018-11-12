// file solver_base_inst.cpp

#include "instantiate.h"
#include "solver_base.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Solver_base; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Solver_base< traits_t >;

} // namespace ns_parametric_search
