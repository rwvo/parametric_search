// file process_base.h

#ifndef PROCESS_BASE_H
#define PROCESS_BASE_H

#include "spawner.h"
#include "spawnable.h"
#include "wrapper.h"

#include <list>

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits >
  class Process_base : public Spawner< Traits >,
		       public Spawnable< Traits >
  //--------------------------------------------------
  {
  public: // type definitions
    // shorthand for "pointer to member function of Process_base,
    // returning void and taking no arguments". This is the type of
    // the member functions of derived classed that can be scheduled
    // by the Scheduler.

    typedef void ( Process_base< Traits >::* unwrapped_memfun_t )();

    // To circumvent a bug in g++ 2.95.3 (it chokes on std::containers
    // of pointers to member functions), we wrap the pointers in a
    // wrapper: this is a simple template class that contains a T,
    // with automatic two-way conversion between T and wrapper< T >.

    typedef wrapper< unwrapped_memfun_t >        memfun_t;
    typedef typename Traits::scheduler_t         scheduler_t;
    typedef typename scheduler_t::process_iter_t process_iter_t;

  public: // member functions
    // the process that creates us must pass us a pointer to itself, so
    // that we can notify our creator when we are finished.

    Process_base( Process_base* parent );
    virtual ~Process_base() = 0;

  protected: // member functions
    // member functions of derived classes are executed by the
    // scheduler; this is done in the order in which they are
    // scheduled by the following function.
    // Use this function in the constructor of derived classes.

    template< class Derived >
    void schedule( void ( Derived::* derived_memfun_p )() );

    // Users of this class (i.e., implementers of derived classes) 
    // need not look below this point.

  private: // member functions
    // make copy constructor and assignment operator inaccessible
    // both are not implemented

    Process_base( const Process_base& );
    Process_base& operator=( const Process_base& );

    // The Scheduler asks a Process to perform the next step of its
    // computation through the following function. Process_base takes
    // care of this. It is not virtual; do not override.

    typedef typename Traits::scheduler_t           scheduler_t;
    friend scheduler_t;

    void resume(); 

    virtual void children_have_finished();

  public: // non-static data members
    std::list< memfun_t > memfuns_;
    typename std::list< memfun_t >::const_iterator memfun_iter_;
    Process_base< Traits >* process_; // not owned
    process_iter_t process_iter_;
  };







  // Definition of template member function
  
  //-------------------------------------------------- 
  template< class Traits >
  template< class Derived >
  void 
  Process_base< Traits >::schedule( void (Derived::* derived_memfun_p)() )
  //-------------------------------------------------- 
  {
    // We must save the "pointer to member function of a derived
    // class". There is no automatic conversion to "pointer to member
    // function of base class", but a static_cast will do (only if
    // there's really a base/derived relation between callee/caller,
    // so the cast is safe.) When we call the member function through
    // the pointer, we must make sure the object on which we call it
    // really provides the references member function, i.e., its
    // dynamic type must be "derived" or "descendant of derived".
    // Note: the call takes place in Process_base_impl::resume()

    memfuns_.push_back( static_cast< unwrapped_memfun_t >
                        ( derived_memfun_p ) );
  }



} // namespace ns_parametric_search


#endif // PROCESS_BASE_H
