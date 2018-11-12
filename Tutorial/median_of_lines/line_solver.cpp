// file line_solver.cpp

#include "line_solver.h"
#include <algorithm>
#include <cassert>

// complete declaration and definition of nested class
//-------------------------------------------------- 
template< class Traits >
class Line_solver< Traits >::Line_comparator
//-------------------------------------------------- 

// function object that compares two lines at a given x_coord.
// operator() returns true if and only if the y_coord of the first
// line is strictly less than the y_coord of the second line.
{
public:
  Line_comparator( number_t x_coord )
    : x_coord_( x_coord ){}
  
  bool operator()( const polynomial_t& lhs, const polynomial_t& rhs ) const
  {
    // compare two lines y = ax + b at a given x_coord:
    // (1) lhs.a * x_coord_ + lhs.b < rhs.a * x_coord_ + rhs.b
    // (2) rewrite
    
    return ( lhs.a - rhs.a ) * x_coord_ < ( rhs.b - lhs.b );
  }
  
private:
  number_t x_coord_;
};




//-------------------------------------------------- 
template< class Traits >
typename Line_solver< Traits >::decision_t
Line_solver< Traits >::decide
( const number_t& x_coord )
//-------------------------------------------------- 
{
  // compute the median line at a given x-coordinate (using the STL
  // algorithm nth_element), and determine whether the median line is
  // below, at, or above zero at that x-coordinate.

  typedef typename std::vector< polynomial_t >::iterator iter_t;
  iter_t median_iter = lines_.begin() +
    ( ( lines_.end() - lines_.begin() ) / 2 );
  assert( median_iter != lines_.end() );
  
  if( median_iter != lines_.begin() )
    std::nth_element( lines_.begin(), median_iter,
  		      lines_.end(), Line_comparator( x_coord ) );
  
  number_t y_coord = median_iter->a * x_coord + median_iter-> b;
  
  if( y_coord < 0 ){
    return ns_parametric_search::Solver_base< Traits >::less;
  }
  else if( y_coord == 0 ){
    return ns_parametric_search::Solver_base< Traits >::equal;
  }
  else {
    return ns_parametric_search::Solver_base< Traits >::greater;
  }
}
