// file scheduler_inst.cpp

#include "instantiate.h"
#include "scheduler.cpp"

namespace ns_parametric_search {

  // Explicitly instantiate Scheduler; traits_t is a typedef
  // for a concrete type, defined in instantiate.h

  template class Scheduler< traits_t >;

} // namespace ns_parametric_search
