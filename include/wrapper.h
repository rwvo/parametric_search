// file wrapper.h

#ifndef WRAPPER_H
#define WRAPPER_H

namespace ns_parametric_search {

  // A wrapper< T > holds a T, and provides automatic conversion
  // between a T and a wrapper< T >. Useless as this may seem, it
  // helps to circumvent a bug in g++ 2.95.3, which chokes on pointers
  // to member functions. Wrapping these pointers makes the problems
  // disappear, while the user doesn't notice the difference between a
  // T and a wrapper< T >. There should be no run-time overhead:
  // sizeof(T) == sizeof( wrapper< T > ), and the two member functions
  // are inline. However, at least with g++ 2.93.3, binaries get a bit
  // larger when using wrappers; probably the compiler doesn't inline
  // very agressively.

  //-------------------------------------------------- 
  template< typename T >
  struct wrapper
  //-------------------------------------------------- 
  {
    wrapper( const T& t ) : t_( t ) {}
    operator T() const { return t_; }
    T t_;
  };

} // namespace ns_parametric_search


#endif // WRAPPER_H
