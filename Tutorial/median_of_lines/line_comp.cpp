// file line_comp.cpp

#include "line_comp.h"
#include <cassert>


// declaration of utilities with translation unit scope
namespace {
  //-------------------------------------------------- 
  template< class Traits >
  void do_deduce( bool& comp_result,
		  const typename Traits::number_t root_value,
		  const typename Traits::polynomial_t& first,
		  const typename Traits::polynomial_t& second,
		  const typename Traits::number_t lower_bound,
		  const typename Traits::number_t upper_bound );
  //-------------------------------------------------- 

    } // unnamed namespace


//--------------------------------------------------
template< class Traits >
Line_comp< Traits >::Line_comp
( process_base_t* parent, bool& comp_result,
  const polynomial_t& first, const polynomial_t& second )
//-------------------------------------------------- 
// save a pointer to the parent process, and save all references to
// variables that we will use later on

  : comparison_base_t( parent ),
    comp_result_( comp_result ),
    first_( first ),
    second_( second ),
    root_value_( static_root_value_ )
{
  typedef typename Traits::root_t root_t;
  ns_parametric_search::Spawner< Traits >::spawn( new root_t( this, root_value_ ) );
}




//--------------------------------------------------
template< class Traits >
Line_comp< Traits >::~Line_comp()
//-------------------------------------------------- 
{
  // EMPTY
}






//--------------------------------------------------
template< class Traits >
typename Traits::comparison_base_t*
Line_comp< Traits >::lazy_create_comparison
( process_base_t* parent, bool& comp_result,
  const polynomial_t& first, const polynomial_t& second )
//-------------------------------------------------- 
{
  // This function is static. Its purpose is to check whether the
  // creation of a Line_comp can be avoided (this is the case if the
  // two lines (polynomials) can be compared using the bounds on the optimal
  // value), and to create a Line_comp otherwise.

  // polynomials are lines: y = ax + b
  if( first.a == second.a ){
    // no proper intersection, so the comparison of the two lines does
    // not depend on the x_coord at which they are compared.
    comp_result = first.b < second.b;
    return 0;
  }

  else {
    // the root is the x-coordinate of the intersection of the two
    // lines

    static_root_value_ = ( second.b - first.b ) / ( first.a - second.a );

    typedef typename Traits::scheduler_t scheduler_t;
    number_t lower_bound = scheduler_t::instance()->lower_bound();
    number_t upper_bound = scheduler_t::instance()->upper_bound();

    if ( static_root_value_ > lower_bound && 
	 static_root_value_ < upper_bound ){
      // the intersection lies within the interval of bounds on the
      // optimal value, but we don't know if the intersection lies to
      // the right or the left of the optimal value. Therefore, we
      // have to resolve the (expensive) decision process. The
      // scheduler decides when this should be done. To this end, we
      // must create a Line_comp for delayed execution.

      return new Line_comp< Traits > ( parent, comp_result,
				       first, second );
    }
    else {
      // The intersection of the lines lies either to the left of the
      // lowerbound on the optimal value, or to the right of the
      // upperbound. The outcome of the comparion can therefore be
      // resolved without doing any expensive computations

      do_deduce< Traits >( comp_result, static_root_value_,
			   first, second,
			   lower_bound, upper_bound );
      return 0;
    }
  }
}




//--------------------------------------------------
template< class Traits >
void
Line_comp< Traits >::deduce
( const number_t& lower_bound,
  const number_t& upper_bound )
//-------------------------------------------------- 
{
  // The scheduler asks us to determine the outcome of the comparison.
  // Precondition (checked in deduce) is that the root does not lie in
  // the interval of bounds on the optimal value.

  do_deduce< Traits >( comp_result_, root_value_,
		       first_, second_,
		       lower_bound, upper_bound );
}



// definition of static data member
//-------------------------------------------------- 
template< class Traits >
typename Line_comp< Traits >::number_t
Line_comp< Traits >::static_root_value_;
//-------------------------------------------------- 


// definition of utilities with translation unit scope
namespace{

  //-------------------------------------------------- 
  template< class Traits >
  void do_deduce( bool& comp_result,
		  const typename Traits::number_t root_value,
		  const typename Traits::polynomial_t& first,
		  const typename Traits::polynomial_t& second,
		  const typename Traits::number_t lower_bound,
		  const typename Traits::number_t upper_bound )
  //-------------------------------------------------- 
  {
    // this function may only be called if the root does not lie within
    // the interval of bounds on the optimal value
    assert( !( root_value > lower_bound && root_value < upper_bound ) );
    
    if ( root_value <= lower_bound ){
      // intersection is ***to the left*** of the optimal value, so
      // the line with the ***smaller*** slope is the smaller at the
      // optimal value.
      comp_result = first.a < second.a;
    }
    else if ( root_value >= upper_bound ){
      // intersection is ***to the right*** of the optimal value, so
      // the line with the ***greater*** slope is smaller at the
      // optimal value.
      comp_result = first.a  > second.a;
    }
  }

} // unnamed namespace
