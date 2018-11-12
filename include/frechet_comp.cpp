// file frechet_comp.cpp

#include "frechet_comp.h"

#include <cmath>
#include <cassert>
#include <algorithm>
#include <cstddef>

// declarations of utility functions with translation unit scope
namespace {

  //-------------------------------------------------- 
  template< class Traits >
  bool compute_squared_root( const typename Traits::polynomial_t& first_, 
			     const typename Traits::polynomial_t& second_, 
			     typename Traits::number_t& squared_root_ );
  //-------------------------------------------------- 

  //-------------------------------------------------- 
  template< class Traits >
  void do_deduce( bool& comp_result,
		  const typename Traits::polynomial_t& first,
		  const typename Traits::polynomial_t& second,
		  const typename Traits::number_t& lower_bound,
		  const typename Traits::number_t& upper_bound );
  //--------------------------------------------------

} // unnamed namespace




//-------------------------------------------------- 
template< class Traits >
Frechet_comp< Traits >::Frechet_comp
( process_base_t* parent, bool& comp_result,
  const polynomial_t& first, const polynomial_t& second )
//--------------------------------------------------
  : comparison_base_t( parent ),
    comp_result_( comp_result ),
    first_( first ),
    second_( second ),
    squared_root_( static_squared_root_ )
{
  typedef typename Traits::root_t root_t;
  this->spawn( new root_t( this, squared_root_ ) );
}




//-------------------------------------------------- 
template< class Traits >
Frechet_comp< Traits >::~Frechet_comp()
//--------------------------------------------------
{
  // EMPTY
}



//-------------------------------------------------- 
template< class Traits >
typename Traits::comparison_base_t* 
Frechet_comp< Traits >::lazy_create_comparison
( process_base_t* parent, bool& comp_result,
  const polynomial_t& first, const polynomial_t& second )
//--------------------------------------------------
{
  // check easy cases first

  // let's define left edges to be smaller than bottom edges,
  // since there's no reason to actually compare them
  if( first.is_left_edge_polynomial && !second.is_left_edge_polynomial ){
    comp_result = true;
    return 0; // no need to create a comparison
  }
  if( !first.is_left_edge_polynomial && second.is_left_edge_polynomial ){
    comp_result = false;
    return 0; // no need to create a comparison
  }

  if( first.is_left_edge_polynomial && first.row != second.row ){
    comp_result = first.row < second.row;
    return 0; // no need to create a comparison
  }

  if( !first.is_left_edge_polynomial && first.column != second.column ){
    comp_result = first.column < second.column;
    return 0; // no need to create a comparison
  }
  // end of easy cases. The real work starts below.


  // if the root lies in the range [lower_bound, upper_bound], we
  // can't deduce the outcome of comparison and we have to create one
  // (that is resolved later by the scheduler). Otherwise, we can
  // actually deduce the outcome.

  typedef typename Traits::scheduler_t scheduler_t;
  number_t lower_bound = scheduler_t::instance()->lower_bound();
  number_t upper_bound = scheduler_t::instance()->upper_bound();

  if( compute_squared_root< Traits >( first, second, static_squared_root_ ) ){
    if( static_squared_root_ > lower_bound &&
	static_squared_root_ < upper_bound ){
      return new Frechet_comp< Traits> ( parent, comp_result,
					 first, second );
    }
  }

  // if we get here, we did not create a comparison object and we can
  // deduce the outcome of the comparison after all.

  do_deduce< Traits >( comp_result, first, second,
		       lower_bound, upper_bound );

  return 0;
}




//-------------------------------------------------- 
template< class Traits >
void
Frechet_comp< Traits >::deduce
( const number_t& lower_bound,
  const number_t& upper_bound )
//--------------------------------------------------
{
  // our (squared) root should be in the range (lower_bound, upper_bound)

  assert( !( squared_root_ > lower_bound && squared_root_ < upper_bound ) );

  // The real work is done elsewhere.

  do_deduce< Traits >( comp_result_, first_, second_,
		       lower_bound, upper_bound );
}




// definition of static data member
//-------------------------------------------------- 
template< class Traits >
typename Frechet_comp< Traits >::number_t
Frechet_comp< Traits >::static_squared_root_;
//-------------------------------------------------- 




// definitions of utility functions with translation unit scope
namespace {

  //-------------------------------------------------- 
  template< class Traits >
  bool 
  compute_squared_root( const typename Traits::polynomial_t& first, 
			const typename Traits::polynomial_t& second, 
			typename Traits::number_t& squared_root )
  //-------------------------------------------------- 
  {
    typedef typename Traits::number_t number_t;
    
    // There is no root if the the projections of boths points onto
    // the segment coincide
    if( first.squared_center == second.squared_center )
      return false;

    number_t value = (second.squared_distance + second.squared_center
		      - first.squared_distance - first.squared_center) /
      ( 2 * ( second.center - first.center ) );

    // now check if the value corresponds to the proper elliptic
    // intersections. If not -> nothing to do

    bool first_is_lower = value < first.center;
    bool second_is_lower = value < second.center;

    if( first.is_lower_bound_polynomial == first_is_lower &&
	second.is_lower_bound_polynomial == second_is_lower ){

      squared_root = ( value - first.center ) *
	( value - first.center ) + first.squared_distance;

      return true;
    }

    return false;
  }



  //-------------------------------------------------- 
  template< class Traits >
  void do_deduce( bool& comp_result,
		  const typename Traits::polynomial_t& first,
		  const typename Traits::polynomial_t& second,
		  const typename Traits::number_t& lower_bound,
		  const typename Traits::number_t& upper_bound )
  //--------------------------------------------------
  {
    // since none of our roots lies in the range (lower_bound,
    // upper_bound), the sign of the polynomial for this comparison (and
    // therefore, the outcome of the comparison) will be the same for
    // any eps in the range. So by evaluating the polynomial for an
    // arbitrary eps in the range, we also know the outcome for
    // eps_star.

    // careful: upper_bound may be infinity (lower_bound should be >= 0)
    // TODO: assert ( lowerbound >= 0 ), if not too expensive

    typedef typename Traits::number_t number_t;
    number_t squared_eps;
    if( lower_bound + 0.123456789 < upper_bound )
      squared_eps = lower_bound + 0.1;
    else
      squared_eps = ( lower_bound + upper_bound ) / 2;
  
    // if the elliptic intersections of both first and second are
    // undefined, we consider them equal. If one of them is defined and
    // the other isn't, the undefined one is smaller. If both are
    // defined, we do an actual comparison.

    if( squared_eps < first.squared_min_eps && 
	squared_eps < second.squared_min_eps ){
      // both are non-existent -> equal
      comp_result = false;
    }
    else if ( squared_eps < first.squared_min_eps ) {
      // only first is non-existent -> smaller than second
      comp_result = true;
    }
    else if ( squared_eps < second.squared_min_eps ) {
      // only second is non-existent -> smaller than first
      comp_result = false;
    }
    else {
      // both are defined
      int first_sign = first.is_lower_bound_polynomial ? -1 : 1;
      int second_sign = second.is_lower_bound_polynomial ? -1 : 1;

      number_t first_val = 
	first.center + 
	first_sign * 
	std::sqrt( squared_eps - first.squared_distance );
      number_t second_val = 
	second.center + 
	second_sign * 
	std::sqrt( squared_eps - second.squared_distance );

      comp_result = ( first_val < second_val );
    }
  }
  
} // unnamed namespace
