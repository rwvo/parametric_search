// file scheduler.cpp

#include "scheduler.h"
#include "delete_object.h"

#include <algorithm>
#include <cassert>
#include <cmath>

// gnu c++ versions below 3.0 don't have <limits>. There's a
// simplework-around: download the compatibility library from
// http://www.boost.org

#if defined (__GNUC__) && __GNUC__ < 3
#  include <boost/limits.hpp>
#else
#  include <limits>
#endif

namespace ns_parametric_search {

  // Declaration of utility functions with file scope
  namespace {

    template< typename number_t > number_t init_lower_bound();
    template< typename number_t > number_t init_upper_bound();
    template< class Traits >
    bool root_is_deducable( typename Traits::root_t* root );

  } // unnamed namespace




  //--------------------------------------------------
  template< class Traits >
  Scheduler< Traits >::Scheduler()
  //--------------------------------------------------
    : active_processes_(),
      suspended_processes_(),
      comparisons_(),
      rootless_comparisons_(),
      roots_(),
      solver_( 0 ),
      running_( false ),
      lower_bound_( init_lower_bound< Scheduler< Traits >::number_t >() ),
      upper_bound_( init_upper_bound< Scheduler< Traits >::number_t >() ),
      no_computations_( 0 ),
      no_processes_( 0 ),
      no_comparisons_( 0 ),
      no_comparisons_total_( 0 ),
      no_roots_( 0 )
  {
    // EMPTY
  }




  //--------------------------------------------------
  template< class Traits >
  Scheduler< Traits >::~Scheduler()
  //--------------------------------------------------
  {
    // We have a member function that takes care of cleaning up (which
    // needs to be done not only at destruction, but also in between
    // consecutive runs of the scheduler). Only the solver object is
    // not cleaned up in cleanup(), since it may be reused in several
    // subsequent runs of the scheduler. Hence, we must delete it here
    // ourselves.

    cleanup();
    delete solver_;
  }





  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::cleanup()
  //--------------------------------------------------
  {
    // Delete all owned objects, except the owned solver object (which
    // may be needed in a subsequent run of the scheduler).

    // A process, comparison, or root that is deleted signals its
    // parent that it has finished if the scheduler is still running.
    // We don't want that to happen during cleanup, so we set running_
    // to false first. This effectively prevents processes from
    // spawning other processes and performing other unnecessary
    // computations.

    running_ = false;

    std::for_each( active_processes_.begin(), active_processes_.end(),
		   Delete_object() );
    std::for_each( suspended_processes_.begin(), suspended_processes_.end(),
		   Delete_object() );
    std::for_each( comparisons_.begin(), comparisons_.end(),
		   Delete_object() );
    std::for_each( rootless_comparisons_.begin(), rootless_comparisons_.end(),
		   Delete_object() );
    std::for_each( roots_.begin(), roots_.end(),
		   Delete_object() );

    active_processes_.clear();
    suspended_processes_.clear();
    comparisons_.clear();
    rootless_comparisons_.clear();
    roots_.clear();
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::reset()
  //--------------------------------------------------
  {
    lower_bound_ = init_lower_bound< Scheduler< Traits >::number_t >();
    upper_bound_ = init_upper_bound< Scheduler< Traits >::number_t >();
    no_computations_ = 0;
    no_processes_ = 0;
    no_comparisons_ = 0;
    no_comparisons_total_ = 0;
    no_roots_ = 0;
  }




  //--------------------------------------------------
  template< class Traits >
  Scheduler< Traits >*
  Scheduler< Traits >::instance()
  //--------------------------------------------------
  {
    return instance_.get();
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::run()
  //--------------------------------------------------
  {
    // PRE: we are not already running
    //
    // As long as there are active processes or deducable roots,
    // run/deduce them; first the active processes, then the roots.
    //
    // The side-effect of running a process is that it is removed from
    // the list of active processes (either because it runs to
    // completion, or it has to wait for child processes or
    // comparisons and is therefore suspended.
    //
    // Deducing the roots may activate some suspended processes;
    // therefore, we loop (i.e., we run active processes again), until
    // there are no more active processes and deducable roots at all.
    // At that point, it's time to find the median root, and resolve
    // it (the expensive way). This makes half of the roots deducable,
    // so we can do the outer loop again.
    //
    // Invariant: deducable roots precede non-deducable roots in the
    // list of roots. We have to do some work to maintain this
    // invariant after resolving the median root the expensive way.

    // sanity check and initialization
    assert( !running_ );
    running_ = true;
    bool aborted( false );

    // main loop
    while( !active_processes_.empty() || !rootless_comparisons_.empty() ||
	   !roots_.empty() ){
      resume_processes_and_deduce_roots();
      find_and_resolve_median_root();
      if( lower_bound_ /* + 1e-12 */ >= upper_bound_ ){
	aborted = true;
	break;
      }
      std::partition( roots_.begin(), roots_.end(),
		      &root_is_deducable< Traits > );
    }
   
    assert( aborted || suspended_processes_.empty() && 
	    rootless_comparisons_.empty() && comparisons_.empty() );
    running_ = false;
    cleanup();
  }







  //--------------------------------------------------
  template< class Traits >
  bool
  Scheduler< Traits >::have_deducable_roots() const
  //--------------------------------------------------
  {
    // If there are any deducable roots, they are at the front of the
    // deque.

    return !roots_.empty() && root_is_deducable< Traits >( *roots_.begin() );
  }
  




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::resume_processes_and_deduce_roots()
  //--------------------------------------------------
  {
    while ( !active_processes_.empty() || !rootless_comparisons_.empty() ||
	    have_deducable_roots() ){
      while( !active_processes_.empty() ){
	( *active_processes_.begin() )->resume();
      }

      while( !rootless_comparisons_.empty() ){
	comparison_base_t* comparison = *rootless_comparisons_.begin();
	rootless_comparisons_.pop_front();
	comparison->deduce( lower_bound_, upper_bound_ );
	delete comparison;
      }

      while( have_deducable_roots() ){
	root_iter_t first_non_deducable = 
	  std::find_if( roots_.begin(), roots_.end(), 
			std::not1( Can_be_deduced() ) );
	std::for_each( roots_.begin(), first_non_deducable,
		       Delete_object() );
	roots_.erase( roots_.begin(), first_non_deducable );
      }
    }
  }





  
  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::resolve_root
  ( const typename Traits::number_t& value )
  //--------------------------------------------------
  {
    assert( solver_ );

    typedef typename Traits::solver_base_t solver_base_t;

    switch( solver_->decide( value ) ){
    case solver_base_t::less:
      lower_bound_ = value;
      break;

    case solver_base_t::equal:
      lower_bound_ = value;
      upper_bound_ = value;
      break;
      
    case solver_base_t::greater:
      upper_bound_ = value;
      break;
      
    default:
      assert( false );
    }
    
  }
  




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::find_and_resolve_median_root()
  //--------------------------------------------------
  {
    if( !roots_.empty() ){

      root_iter_t  median_root_iter = roots_.begin() + 
	( ( roots_.end() - roots_.begin() ) / 2 );
      assert( median_root_iter != roots_.end() );

      if( median_root_iter != roots_.begin() )
	std::nth_element( roots_.begin(), median_root_iter, 
			  roots_.end(), Root_comp() );

      assert( median_root_iter !=  roots_.end() );

      root_t* median_root = *median_root_iter;
      roots_.erase( median_root_iter );

      resolve_root( median_root->value() );
      delete median_root;

      ++no_computations_;
    }
  }


  



  //--------------------------------------------------
  template< class Traits >
  bool
  Scheduler< Traits >::is_running() const
  //--------------------------------------------------
  {
    return running_;
  }




  //--------------------------------------------------
  template< class Traits >
  const typename Traits::number_t&
  Scheduler< Traits >::lower_bound() const
  //--------------------------------------------------
  {
    return lower_bound_;
  }




  //--------------------------------------------------
  template< class Traits >
  const typename Traits::number_t&
  Scheduler< Traits >::upper_bound() const
  //--------------------------------------------------
  {
    return upper_bound_;
  }




  //--------------------------------------------------
  template< class Traits >
  std::size_t
  Scheduler< Traits >::no_computations() const
  //--------------------------------------------------
  {
    return no_computations_;
  }




  //--------------------------------------------------
  template< class Traits >
  std::size_t
  Scheduler< Traits >::no_processes() const
  //--------------------------------------------------
  {
    return no_processes_;
  }




  //--------------------------------------------------
  template< class Traits >
  std::size_t
  Scheduler< Traits >::no_comparisons() const
  //--------------------------------------------------
  {
    return no_comparisons_;
  }




  //--------------------------------------------------
  template< class Traits >
  std::size_t
  Scheduler< Traits >::no_comparisons_total() const
  //--------------------------------------------------
  {
    return no_comparisons_total_;
  }




  //--------------------------------------------------
  template< class Traits >
  std::size_t
  Scheduler< Traits >::no_roots() const
  //--------------------------------------------------
  {
    return no_roots_;
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::inc_no_comparisons_total() 
  //--------------------------------------------------
  {
    ++no_comparisons_total_;
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::increase_lower_bound_to( const number_t& bound )
  //--------------------------------------------------
  {
    lower_bound_ = std::max( lower_bound_, bound );
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::decrease_upper_bound_to( const number_t& bound )
  //--------------------------------------------------
  {
    upper_bound_ = std::min( upper_bound_, bound );
  }




  //--------------------------------------------------
  template< class Traits >
  typename Scheduler< Traits >::process_iter_t 
  Scheduler< Traits >::schedule( process_base_t* process )
  //--------------------------------------------------
  {
    // insert the process pointer into the appropriate container, and
    // return and iterator pointing to the inserted element

    ++no_processes_;
    return active_processes_.insert( active_processes_.end(), process );
  }



  //--------------------------------------------------
  template< class Traits >
  typename Scheduler< Traits >::comparison_iter_t 
  Scheduler< Traits >::schedule( comparison_base_t* comparison )
  //--------------------------------------------------
  {
    // insert the comparison pointer into the appropriate container, and
    // return and iterator pointing to the inserted element

    ++no_comparisons_;
    return comparisons_.insert( comparisons_.end(), comparison );
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::schedule( root_t* root )
  //--------------------------------------------------
  {
    // Insert the root pointer into the appropriate container. Roots
    // that can be deduced go at the front, and the ones that can't be
    // deduced go at the back. This way, our member function 'run' can
    // easily determine if there are roots that can be deduced
    // efficiently, before it attempts to compute a root the expensive way.

    if( root_is_deducable< Traits >( root ) ){
      roots_.push_front( root );
    }
    else {
      roots_.push_back( root );
    }

    ++no_roots_;
  }




  //--------------------------------------------------
  template< class Traits >
  typename Scheduler< Traits >::process_iter_t
  Scheduler< Traits >::suspend( process_iter_t process_iter )
  //--------------------------------------------------
  {
    // PRE: process_iter is a dereferenceable iterator in
    //      active_processes_.

    // remove the process from the list of active processes, insert it
    // into the list of suspended processes, and return an iterator to
    // the process
    
    process_base_t* process = *process_iter;
    active_processes_.erase( process_iter );
    return suspended_processes_.insert( suspended_processes_.end(), process );
  }




  //--------------------------------------------------
  template< class Traits >
  typename Scheduler< Traits >::process_iter_t
  Scheduler< Traits >::reactivate( process_iter_t process_iter )
  //--------------------------------------------------
  {
    // PRE: process_iter is a dereferenceable iterator in
    //      suspended_processes_.

    // remove the process from the list of suspended processes, insert
    // it into the list of active processes, and return an iterator to
    // the process
    
    process_base_t* process = *process_iter;
    suspended_processes_.erase( process_iter );
    return active_processes_.insert( active_processes_.end(), process );
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::terminate( process_iter_t process_iter )
  //--------------------------------------------------
  {
    // PRE: process_iter is a dereferenceable iterator in
    //      active_processes_.

    // remove the process pointer from the list of active processes
    // and delete the process (so that its destructor can do its job)
    
    delete *process_iter;
    active_processes_.erase( process_iter );
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::reactivate( comparison_iter_t comparison_iter )
  //--------------------------------------------------
  {
    // Have the comparison resolve itself; after that, it's done, so
    // we delete it.
    
    (*comparison_iter)->deduce( lower_bound_, upper_bound_ );
    delete *comparison_iter;
    comparisons_.erase( comparison_iter );
  }




  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::rootless( comparison_iter_t comparison_iter )
  //--------------------------------------------------
  {
    // Move the comparison to the list of rootless comparisons
    // PRE: comparison_iter is a dereferenceable iterator in
    //      comparisons_.

    // remove the comparison from the list of comparisons, insert it
    // into the list of rootless comparison
    
    comparison_base_t* comparison = *comparison_iter;
    comparisons_.erase( comparison_iter );
    rootless_comparisons_.push_back( comparison );
  }


  //--------------------------------------------------
  template< class Traits >
  void
  Scheduler< Traits >::register_solver( solver_base_t* solver )
  //--------------------------------------------------
  {
    // register a new solver; delete the old one (if any) first
    delete solver_;
    solver_ = solver;
  }



  // definition of static member
  template< class Traits>
  std::unique_ptr< Scheduler< Traits > >
  Scheduler< Traits >::instance_( new Scheduler< Traits > );



  //--------------------------------------------------
  // definition of nested class member functions


  //--------------------------------------------------
  template< class Traits >
  bool
  Scheduler< Traits >::Can_be_deduced::operator()
    ( root_t* root ) const
  //--------------------------------------------------
  {
    return root_is_deducable< Traits >( root );
  }




  //--------------------------------------------------
  template< class Traits >
  bool
  Scheduler< Traits >::Root_comp::operator()
    ( root_t* lhs,  root_t* rhs ) const
  //--------------------------------------------------
  {
    return lhs->value() < rhs->value();
  }




  //--------------------------------------------------
  // Utility functions with file scope
  namespace {


    //--------------------------------------------------
    template< typename number_t >
    number_t
    init_lower_bound()
    //--------------------------------------------------
    {
      return -init_upper_bound< number_t >();
    }



    //--------------------------------------------------
    template< typename number_t >
    number_t
    init_upper_bound()
    //--------------------------------------------------
    {
      // initialize to an obviously wrong value so that we can check
      // at the end of this function whether we really computed
      // something useful

      number_t upper_bound( 0 );

      // if number_t has some representation of infinity, initialze to
      // that, otherwise compute the maximum possible value `by hand'.

      if ( std::numeric_limits< number_t >::is_specialized ){
	assert( !std::numeric_limits< number_t >::is_integer );
	if ( std::numeric_limits< number_t >::has_infinity ) {
	  upper_bound = std::numeric_limits< number_t >::infinity();
	  
	  // there's a bug in STLport 4.0: for little endian machines,
	  // the bit pattern for infinity is incorrectly specified.
	  // Check this by testing if upper_bound really behaves as
	  // infinity

	  assert( upper_bound == upper_bound + 1.0 );
	}
	else { 
	  // no infinity value available; calculate max possible value
	  upper_bound = static_cast< number_t >
	    ( std::pow
	      ( static_cast< double >
		( std::numeric_limits< number_t >::radix ),
		std::numeric_limits< number_t >::max_exponent - 1 ) );
	}
      }
      else { // no specialization of numeric_limits for number_t

	// for now, we *require* specialization of numeric_limits< number_t >
	// we could consider allowing the user to specify the values for
	// upper_bound (and lower_bound) in a Traits class

	upper_bound = static_cast< number_t >
	  ( std::pow
	    ( static_cast< double >
	      ( std::numeric_limits< double >::radix ),
	      std::numeric_limits< double >::max_exponent - 1 ) );

      }
      
      // check if we really computed something useful
      assert( upper_bound > 0 );

      return upper_bound;
    }

    //-------------------------------------------------- 
    template< class Traits >
    bool root_is_deducable( typename Traits::root_t* root )
    //-------------------------------------------------- 
    {
      typedef typename Traits::scheduler_t scheduler_t;

      return  root->value() <= scheduler_t::instance()->lower_bound()  ||
	root->value() >= scheduler_t::instance()->upper_bound() ;
    }



  } // unnamed namespace
  //-------------------------------------------------- 


} // namespace ns_parametric_search

