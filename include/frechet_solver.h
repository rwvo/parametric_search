// file frechet_solver.h

#ifndef FRECHET_SOLVER_H
#define FRECHET_SOLVER_H

#include "solver_base.h"
#include "frechet_cell.h"
#include "frechet_polynomial.h"
#include <vector>

//--------------------------------------------------
template< typename Traits >
class Frechet_solver : public Traits::solver_base_t
//--------------------------------------------------
{
public: // type declarations
  typedef typename Traits::number_t                   number_t;
  typedef typename Traits::solver_base_t              solver_base_t;
  typedef typename solver_base_t::decision_t          decision_t;
  typedef typename Frechet_cell< Traits >::Point      Point;
  typedef typename Frechet_cell< Traits >::Segment    Segment;
  typedef std::vector< Point >                        Polyline;

public:
  template< typename InputIter >
  Frechet_solver( const InputIter& polyline_h_first,
		  const InputIter& polyline_h_last,
		  const InputIter& polyline_v_first,
		  const InputIter& polyline_v_last );

  std::vector< Frechet_polynomial< Traits > > frechet_polynomials() const;
  number_t squared_min_eps() const;

  virtual decision_t decide( const number_t& squared_eps );

private:
  void calc_ellips_intersections( const number_t& squared_eps );
  void calc_monotone_reachable_bounds( const number_t& squared_eps );

private:
  Polyline polyline_h_;
  Polyline polyline_v_;
  std::size_t no_columns_;
  std::size_t no_rows_;
  number_t squared_min_eps_;
  std::vector< std::vector< Frechet_cell< Traits > > > frechet_matrix_;
};


// definition of template constructor, plus declarations of used
// utitlity functions therein


// declarations of utility functions with translation unit scope
namespace {

  //--------------------------------------------------
  template< typename Traits >
  bool has_zero_length_segments
  ( const typename Frechet_solver< Traits >::Polyline& polyline );
  //--------------------------------------------------


  //--------------------------------------------------
  template< typename Traits >
  typename Traits::number_t calc_squared_min_eps
  ( const typename Frechet_solver< Traits >::Polyline& polyline_h,
    const typename Frechet_solver< Traits >::Polyline& polyline_v);
  //--------------------------------------------------


} // unnamed namespace


//-------------------------------------------------- 
template< typename Traits >
template <typename InputIter >
Frechet_solver< Traits >::Frechet_solver
( const InputIter& polyline_h_first, 
  const InputIter& polyline_h_last, 
  const InputIter& polyline_v_first,
  const InputIter& polyline_v_last )
//--------------------------------------------------
  : polyline_h_( polyline_h_first, polyline_h_last ),
    polyline_v_( polyline_v_first, polyline_v_last ),
    no_columns_( polyline_h_.size() ),
    no_rows_( polyline_v_.size() ), 
    squared_min_eps_( calc_squared_min_eps< Traits >
		      ( polyline_h_, polyline_v_ ) ),
    frechet_matrix_( no_columns_ )
{
  // check preconditions
  assert( polyline_h_.size() > 1 &&
	  polyline_v_.size() > 1 &&
	  !has_zero_length_segments< Traits >( polyline_h_ ) &&
	  !has_zero_length_segments< Traits >( polyline_v_ ) );

  // reserve space for rows
  typedef typename 
    std::vector< std::vector< Frechet_cell< Traits > > >::iterator iter_t;
  for( iter_t it = frechet_matrix_.begin();
       it != frechet_matrix_.end();
       ++it ){
    it->reserve( no_rows_ );
  }

  //-------------------------------------------------- 
  // deal with all columns but the last
  for( std::size_t column = 0; column < no_columns_ - 1; ++column ){
    Segment bottom_segment( polyline_h_[ column ], polyline_h_[ column+1 ] );
    // for all rows but the last
    for( std::size_t row = 0; row < no_rows_ - 1; ++row ){
      Segment left_segment( polyline_v_[ row ], polyline_v_[ row+1 ] );

      frechet_matrix_[ column ].push_back
	( Frechet_cell< Traits >( left_segment, polyline_h_[ column ],
				    bottom_segment, polyline_v_[ row ] ) );
    }

    // last row has a phantom left edge
    frechet_matrix_[ column ].push_back
      ( Frechet_cell< Traits >( typename Frechet_cell< Traits >::phantom_edge(),
				  bottom_segment, polyline_v_[ no_rows_ - 1 ] ) );
  }

  //-------------------------------------------------- 
  // deal with the last column
  // for all rows but the last
  for( std::size_t row = 0; row < no_rows_ - 1; ++row ){
    Segment left_segment( polyline_v_[ row ], polyline_v_[ row+1 ] );
    
    frechet_matrix_[ no_columns_ - 1 ].push_back
      ( Frechet_cell< Traits >( left_segment, polyline_h_[ no_columns_ - 1 ],
				typename Frechet_cell< Traits >::phantom_edge() ) );
  }

  // last row has a phantom left edge
  frechet_matrix_[ no_columns_ - 1 ].push_back
    ( Frechet_cell< Traits >( typename Frechet_cell< Traits >::phantom_edge(), 
			      typename Frechet_cell< Traits >::phantom_edge() ) );
}



namespace {

  //-------------------------------------------------- 
  template< typename Traits >
  bool has_zero_length_segments
  ( const typename Frechet_solver< Traits >::Polyline& polyline )
  //-------------------------------------------------- 
  {
    if( polyline.size() <= 1 ) 
      return false;

    typedef typename 
      Frechet_solver< Traits >::Polyline::const_iterator iter_t;
    iter_t it1 = polyline.begin();
    iter_t it2 = it1 + 1;

    while( it2 != polyline.end() ){
      if( *it1 == *it2 ) 
	return true;
      ++it1;
      ++it2;
    }

    return false;
  }


  //-------------------------------------------------- 
  template< typename Traits >
  typename Traits::number_t calc_squared_min_eps
  ( const typename Frechet_solver< Traits >::Polyline& polyline_h,
    const typename Frechet_solver< Traits >::Polyline& polyline_v)
  //-------------------------------------------------- 
  {
    // return the maximum of the distances between the two starting
    // and ending points of the two polygonal chains. The Frechet distance is
    // at least this value.

    assert( !polyline_h.empty() &&
	    !polyline_v.empty() );

    typename Traits::number_t squared_distance_at_start = 
      CGAL::squared_distance( polyline_h.front(), polyline_v.front() );
    typename Traits::number_t squared_distance_at_end = 
      CGAL::squared_distance( polyline_h.back(), polyline_v.back() );

    return std::max( squared_distance_at_start,
		     squared_distance_at_end );
  }



} // unnamed namespace


#endif // FRECHET_SOLVER_H
