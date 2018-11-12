// file comparison_base_inst.cpp

#include "instantiate.h"
#include "comparison_base.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Comparison_base; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Comparison_base< traits_t >;

} // namespace ns_parametric_search
