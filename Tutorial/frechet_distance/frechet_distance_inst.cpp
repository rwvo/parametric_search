// file frechet_distance_inst.cpp

#include "instantiate.h"
#include "frechet_distance.cpp"

#include <vector>

// Explicitly instantiate Frechet_distance; traits_t is a typedef for a
// concrete type, defined in instantiate.h 

typedef std::vector< traits_t::Point >::iterator iter_t;

template
void frechet_distance< traits_t, iter_t >
( const iter_t& first_polyline_first,
  const iter_t& first_polyline_last,
  const iter_t& second_polyline_first,
  const iter_t& second_polyline_last );


