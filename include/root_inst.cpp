// file root_inst.cpp

#include "instantiate.h"
#include "root.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Root; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Root< traits_t >;

} // namespace ns_parametric_search
