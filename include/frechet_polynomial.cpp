// file frechet_polynomial.cpp

#include "frechet_polynomial.h"
#include <cassert>

//--------------------------------------------------
template< typename Traits >
Frechet_polynomial< Traits >::Frechet_polynomial
( std::size_t column, std::size_t row,
  bool is_left_edge_polynomial, bool is_lower_bound_polynomial,
  const number_t& squared_distance,
  const number_t& distance,
  const number_t& squared_center,
  const number_t& center,
  const number_t& squared_min_eps )
//--------------------------------------------------     
  : column( column ),
    row( row ),
    is_left_edge_polynomial( is_left_edge_polynomial ),
    is_lower_bound_polynomial( is_lower_bound_polynomial ),
    squared_distance( squared_distance ),
    distance( distance ),
    squared_center( squared_center ),
    center( center ),
    squared_min_eps( squared_min_eps )
{
  // EMPTY
}

