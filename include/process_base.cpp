// file process_base.cpp

#include "process_base.h"
#include <cassert>

namespace ns_parametric_search {

  // Constructor
  //-------------------------------------------------- 
  template< class Traits >
  Process_base< Traits >::Process_base( Process_base* parent )
  //--------------------------------------------------
    // Initialize base classes, create an instance of our implementation
    // class and save it
    : Spawner< Traits >(),
      Spawnable< Traits >( parent ),
      memfuns_(),
      memfun_iter_(),
      started_(false),
      // register ourselves with the Scheduler, and save the
      // iterator that the Scheduler returns
      process_iter_( scheduler_t::instance()->schedule( this ) )
  {
    // EMPTY 
  }




  // Destructor
  //--------------------------------------------------
  template< class Traits >
  Process_base< Traits >::~Process_base()
  //--------------------------------------------------
  {
    // EMPY: our data members are smart enough to cleanup 
    // themselves.
  }




  //-------------------------------------------------- 
  template< class Traits >
  void
  Process_base< Traits >::resume()
  //-------------------------------------------------- 
  {
    // Precondition: no child processes or comparisons spawned by us
    // should still be active.
    assert( !Spawner< Traits >::active_child_count() );


    // if we haven't called any scheduled member function yet, make
    // memfun_iter_ point to the first one
    if( !started_ ){
      memfun_iter_ = memfuns_.begin();
      started_ = true;
    }
    
    // call the scheduled member functions as long as we have no
    // active child processes or comparisons 
    while( !Spawner< Traits >::active_child_count() && 
	   memfun_iter_ != memfuns_.end() ){
      memfun_t memfun = *( memfun_iter_ );
      ( this->*memfun )();
      ++memfun_iter_;
    }

    // Now either we're completely finished (ask the scheduler to get
    // rid of us), or we're waiting for active child
    // processes/comparisons (ask the scheduler to suspend us)
    // CAREFUL: both conditions can be true simultaneously (i.e., the last 
    // schedeled member function can have spawned child processes.
    if( !Spawner< Traits >::active_child_count() && memfun_iter_ == memfuns_.end() ){
      scheduler_t::instance()->terminate( process_iter_ );
    }
    else{
      assert( Spawner< Traits >::active_child_count() );
      process_iter_ = scheduler_t::instance()->suspend( process_iter_ );
    }
    // no more statements in this function after this point--we could
    // be destructed
  }




  //--------------------------------------------------
  template< class Traits >
  void 
  Process_base< Traits >::children_have_finished()
  //-------------------------------------------------- 
  {
    // no more children to wait for -> reactivate process
    process_iter_ = scheduler_t::instance()->reactivate( process_iter_ );
  }

} // namespace ns_parametric_search

