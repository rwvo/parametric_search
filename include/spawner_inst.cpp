// file spawner_inst.cpp

#include "instantiate.h"
#include "spawner.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Spawner; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Spawner< traits_t >;

} // namespace ns_parametric_search
