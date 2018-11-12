// file process_base_inst.cpp

#include "instantiate.h"
#include "process_base.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Process_base; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Process_base< traits_t >;

} // namespace ns_parametric_search
