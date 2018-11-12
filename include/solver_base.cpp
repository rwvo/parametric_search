// file solver_base.cpp

#include "solver_base.h"

namespace ns_parametric_search {

  //-------------------------------------------------- 
  template< class Traits >
  Solver_base< Traits >::Solver_base()
  //-------------------------------------------------- 
  {
    // when a solver is created, it must be registered with the
    // Scheduler.

    typedef typename Traits::scheduler_t scheduler_t;
    scheduler_t::instance()->register_solver( this );
  }





  //-------------------------------------------------- 
  template< class Traits >
  Solver_base< Traits >::~Solver_base()
  //-------------------------------------------------- 
  {
    // Empty
  }

} // namespace ns_parametric_search
