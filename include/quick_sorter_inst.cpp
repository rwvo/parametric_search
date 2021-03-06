// file quick_sorter_inst.cpp

#include "instantiate.h"
#include "quick_sorter.cpp"
#include "vector"

namespace ns_parametric_search {

  // Explicitly instantiate Quick_sorter; traits_t is a typedef
  // for a concrete type, defined in instantiate.h
  typedef std::vector< traits_t::polynomial_t >::iterator iter_t;
  template class Quick_sorter< traits_t, iter_t >;

} // namespace ns_parametric_search
