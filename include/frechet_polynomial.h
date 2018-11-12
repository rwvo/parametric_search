// file frechet_polynomial.h

#ifndef FRECHET_POLYNOMIAL_H
#define FRECHET_POLYNOMIAL_H

#include <cstddef>

//--------------------------------------------------
template< typename Traits >
struct Frechet_polynomial
//--------------------------------------------------
{
  // type declaration
  typedef typename Traits::number_t number_t;

  // convenience constructor
  Frechet_polynomial( std::size_t column, std::size_t row,
		      bool is_left_edge_polynomial, 
		      bool is_lower_bound_polynomial,
		      const number_t& squared_distance,
		      const number_t& distance,
		      const number_t& squared_center,
		      const number_t& center,
		      const number_t& squared_min_eps );

  std::size_t column;
  std::size_t row;
  bool is_left_edge_polynomial;
  bool is_lower_bound_polynomial;
  number_t squared_distance;
  number_t distance;
  number_t squared_center;
  number_t center;
  number_t squared_min_eps;
};

#endif // FRECHET_POLYNOMIAL_H
