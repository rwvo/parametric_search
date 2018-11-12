// file spawner.cpp

#include "spawner.h"
#include <cassert>

namespace ns_parametric_search {

  //-------------------------------------------------- 
  template< class Traits >
  Spawner< Traits >::Spawner()
  //-------------------------------------------------- 
    : active_child_count_( 0 )
  {
    // EMPTY
  }


  //-------------------------------------------------- 
  template< class Traits >
  Spawner< Traits >::~Spawner()
  //-------------------------------------------------- 
  {
    // EMPTY
  }


  //-------------------------------------------------- 
  template< class Traits >
  void
  Spawner< Traits >::spawn( Spawnable< Traits >* child )
  //-------------------------------------------------- 
  {
    // The child Spawnable (i.e., Process, Comparison, or Root)
    // registers itself with the Scheduler. The only thing we need to
    // do is to keep track how many children are still active.
    if( child )
      ++active_child_count_;
  }


  //-------------------------------------------------- 
  template< class Traits >
  void
  Spawner< Traits >::child_has_finished()
  //-------------------------------------------------- 
  {
    // Precondition: the number of active child processes 
    // or comparisons should be greater than 0.
    assert( active_child_count_ );
    
    // we are notified that a child has finished, so we should
    // decrease the count. If it's zero, take (virtual) action (i.e.,
    // derived classes determine what to do when *all* children have
    // finished.
    if ( --active_child_count_ == 0 ){
      children_have_finished();
    }
  }



  //-------------------------------------------------- 
  template< class Traits >
  std::size_t
  Spawner< Traits >::active_child_count() const
  //-------------------------------------------------- 
  {
    return active_child_count_;
  }


} // namespace ns_parametric_search

