// file spawnable_inst.cpp

#include "instantiate.h"
#include "spawnable.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Spawnable; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Spawnable< traits_t >;

} // namespace ns_parametric_search
