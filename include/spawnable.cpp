// file spawnable.cpp
#include "spawnable.h"
#include "spawner.h"

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits >
  Spawnable< Traits >::Spawnable
  ( Spawner< Traits >* parent )
  //--------------------------------------------------
    // save the pointer to our calling process
    : parent_( parent )
  {
    // EMPTY
    // ++created;
  }



  //--------------------------------------------------
  template< class Traits >
  Spawnable< Traits >::~Spawnable()
  //--------------------------------------------------
  {
    // Notify our spawning process/comparison that we are done, but
    // only if the scheduler is still running. The `top-level process'
    // has no such spawning process, so we should test if parent_
    // really points to something (for the top-level process it's a
    // null pointer).
    typedef typename Traits::scheduler_t scheduler_t;
    if( scheduler_t::instance()->is_running() && parent_ ){
      parent_->child_has_finished();
    }
    // We don't own the Spawner pointed to by parent_, so we should
    // *not* delete it.

    // ++destroyed;
  }


//    //--------------------------------------------------
//    template< class Traits >
//    void Spawnable< Traits >::stats()
//    //--------------------------------------------------
//    {
//      std::cout << "Spawnables created:   " << created
//  	      << std::endl
//  	      << "Spawnables destroyed: " << destroyed
//  	      << std::endl;
//    }

//    // definitions of static members
//    template< class Traits >
//    std::size_t Spawnable< Traits >::created = 0;

//    template< class Traits >
//    std::size_t Spawnable< Traits >::destroyed = 0;


} // namespace ns_parametric_search


