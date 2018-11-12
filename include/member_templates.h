// file member_templates.h

#ifndef MEMBER_TEMPLATES_H
#define MEMBER_TEMPLATES_H

#include "process_base.h"

namespace ns_parametric_search {

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

#endif // MEMBER_TEMPLATES_H
