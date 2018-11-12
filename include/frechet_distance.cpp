// file frechet_distance.cpp

#include "frechet_distance.h"
#include "frechet_preprocessor.h"
#include "quick_sorter.h"
#include <vector>

template< typename Traits, typename InputIter >
void frechet_distance( const InputIter& first_polyline_first,
		       const InputIter& first_polyline_last,
		       const InputIter& second_polyline_first,
		       const InputIter& second_polyline_last )
{
  typedef Frechet_solver< Traits > solver_t;
 
  //-------------------------------------------------- 
  // declare a decision problem solver. It must be a dynamic object
  // (i.e., created with `new'), and it will be owned by the
  // framework (we must not delete it ourselves).

  solver_t* solver = new solver_t( first_polyline_first,
				   first_polyline_last,
				   second_polyline_first,
				   second_polyline_last );
  
  //-------------------------------------------------- 
  // There is one easy case to check: the Frechet distance is at least
  // the distance between the startpoints/endpoints of the two
  // polylines. The solver computes this distance (or rather, the
  // square of it; we avoid taking square roots as much as possible
  // for efficiency reasons), and stores it as a lower bound on
  // lambda-star. We first test if this value is the actual Frechet
  // distance.

  if( solver->decide( solver->squared_min_eps() ) == solver_t::equal ){

    pms::Scheduler< Frechet_traits >::instance()->
      increase_lower_bound_to( solver->squared_min_eps() );
    pms::Scheduler< Frechet_traits >::instance()->
      decrease_upper_bound_to( solver->squared_min_eps() );
  }
  else {
    std::vector< Frechet_traits::polynomial_t > 
      polynomials( solver->frechet_polynomials() );

    typedef std::vector< Frechet_traits::polynomial_t >::iterator iter_t;
    typedef pms::Frechet_preprocessor< Frechet_traits, iter_t > preprocessor;
    typedef pms::Quick_sorter< Frechet_traits, iter_t > sorter;
    
    new preprocessor(  0 , polynomials.begin(), polynomials.end() );
    new sorter( 0 , polynomials.begin(), polynomials.end() );
    
    pms::Scheduler< Frechet_traits >::instance()->
      increase_lower_bound_to( solver->squared_min_eps() );

    pms::Scheduler< Frechet_traits >::instance()->run();
  }    
}

