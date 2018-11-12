// file frechet_solver.cpp

#include "frechet_solver.h"
#include <CGAL/squared_distance_2.h>
#include <cstddef>

#include <iostream> // DEBUG
#include <cmath>    // DEBUG

// declarations of utility functions with translation unit scope
namespace {

  //-------------------------------------------------- 
  template< typename Traits >
  void get_frechet_polynomial
  ( const Frechet_cell_edge< Traits >& edge,
    std::size_t column, std::size_t row,
    bool is_left_edge_polynomial,
    std::vector< Frechet_polynomial< Traits > >& polynomials );
  //-------------------------------------------------- 

} // unnamed namespace



//-------------------------------------------------- 
template< typename Traits >
std::vector< Frechet_polynomial< Traits > >
Frechet_solver< Traits >::frechet_polynomials() const
//--------------------------------------------------
{
  std::vector< Frechet_polynomial< Traits > > polynomials;

  for( std::size_t column = 0; column != no_columns_; ++column )
    for( std::size_t row = 0; row != no_rows_; ++row ){
      get_frechet_polynomial< Traits >
	( frechet_matrix_[ column ][ row ].left_edge(), 
	  column, row, true, polynomials );
      get_frechet_polynomial< Traits >
	( frechet_matrix_[ column ][ row ].bottom_edge(), 
	  column, row, false, polynomials );
    }

  return polynomials;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_solver< Traits >::squared_min_eps() const
//--------------------------------------------------
{
  return squared_min_eps_;
}





//-------------------------------------------------- 
template< typename Traits >
Frechet_solver< Traits >::decision_t
Frechet_solver< Traits >::decide( const number_t& squared_eps )
//--------------------------------------------------
{
  if( squared_eps < squared_min_eps_ ){
    return less;
  }
  
  calc_ellips_intersections( squared_eps );
  calc_monotone_reachable_bounds( squared_eps );
  
  Frechet_cell_edge< Traits > right_edge = 
    frechet_matrix_[ no_columns_ - 1 ][ no_rows_ - 2 ].left_edge();
  // Frechet_cell_edge< number_t > top_edge = 
  //  frechet_matrix_[ no_columns_ - 2 ][ no_rows_ - 1 ].bottom_edge();

  decision_t decision;

  if( right_edge.is_empty() 
      || right_edge.upper_bound() < right_edge.length() ){
    // this assertion sometimes fails for doubles due to rounding errors.
    // assert( top_edge.normalized_upper_bound() < 1 );

    decision = less;
   }

  else{
    // this assertion sometimes fails for doubles due to rounding errors.
    // assert( top_edge.normalized_upper_bound() == 1 );
    if ( right_edge.path_has_slack() && squared_eps > squared_min_eps_ )
      decision = greater;
    else
      decision = equal;
  }

  return decision;
}




//-------------------------------------------------- 
template< typename Traits >
void
Frechet_solver< Traits >::calc_ellips_intersections
( const number_t& squared_eps )
//--------------------------------------------------
{
  for( std::size_t column = 0; column != no_columns_; ++column ){
    for( std::size_t row = 0; row != no_rows_; ++row ){
      frechet_matrix_[ column ][ row ].calc_ellips_intersections
	( squared_eps );
    }
  }
}




//-------------------------------------------------- 
template< typename Traits >
void
Frechet_solver< Traits >::calc_monotone_reachable_bounds
( const number_t& squared_eps )
//--------------------------------------------------
{
  for( std::size_t column = 0; column != no_columns_; ++column )
    for( std::size_t row = 0; row != no_rows_; ++row ){
      Frechet_cell< Traits >* left = 
	column ?  &frechet_matrix_[ column - 1 ][ row ] : 0;
      Frechet_cell< Traits >* below = 
	row ? &frechet_matrix_[ column ][ row - 1 ] : 0;

      frechet_matrix_[ column ][ row ].calc_monotone_reachable_bounds
	( squared_eps, left, below );
    }
}





//-------------------------------------------------- 
// utility functions with translation unit scope

namespace {


  //-------------------------------------------------- 
  template< typename Traits >
  void get_frechet_polynomial
  ( const Frechet_cell_edge< Traits >& edge,
    std::size_t column, std::size_t row,
    bool is_left_edge_polynomial,
    std::vector< Frechet_polynomial< Traits > >& polynomials )
  //-------------------------------------------------- 
  {
    if( !edge.is_phantom_edge() ){
      const bool is_lower_bound_polynomial( true );

      polynomials.push_back( Frechet_polynomial< Traits >
		       ( column, row, 
			 is_left_edge_polynomial, is_lower_bound_polynomial,
			 edge.squared_distance(),
			 edge.distance(),
			 edge.squared_center(),
			 edge.center(),
			 edge.squared_min_eps() ) );
			 
      polynomials.push_back( Frechet_polynomial< Traits >
		       ( column, row, 
			 is_left_edge_polynomial, !is_lower_bound_polynomial,
			 edge.squared_distance(),
			 edge.distance(),
			 edge.squared_center(),
			 edge.center(),
			 edge.squared_min_eps() ) );
    }
  }



} // unnamed namespace
