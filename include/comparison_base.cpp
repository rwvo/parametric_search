// file comparison_base.cpp

#include "comparison_base.h"

namespace ns_parametric_search {

  // Constructor
  //-------------------------------------------------- 
  template< class Traits >
  Comparison_base< Traits >::Comparison_base
  (  Comparison_base< Traits >::process_base_t* parent )
  //--------------------------------------------------
    // Initialize base classes and register ourselves with the scheduler 
    : Spawner< Traits >(),
      Spawnable< Traits >( parent ),
      comparison_iter_( scheduler_t::instance()->schedule( this ) )

  {
    // EMPTY
  }



  // Destructor
  //-------------------------------------------------- 
  template< class Traits >
  Comparison_base< Traits >::~Comparison_base()
  //--------------------------------------------------
  {
  }

  

  
  template< class Traits >
  void
  Comparison_base< Traits >::nothing_spawned()
  //--------------------------------------------------
  {
    // Tell the scheduler that we can be resolved
    scheduler_t::instance()->rootless( comparison_iter_ );
  }




  //-------------------------------------------------- 
  template< class Traits >
  void
  Comparison_base< Traits >::children_have_finished()
  //--------------------------------------------------
  {
    // Tell the scheduler that we can be resolved
    scheduler_t::instance()->reactivate( comparison_iter_ );
  }

  

  


} // namespace ns_parametric_search

