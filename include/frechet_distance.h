// file frechet_distance.h

#ifndef FRECHET_DISTANCE_H
#define FRECHET_DISTANCE_H

#include "instantiate.h"

template< typename Traits, typename InputIter >
void frechet_distance( const InputIter& first_polyline_first,
		       const InputIter& first_polyline_last,
		       const InputIter& second_polyline_first,
		       const InputIter& second_polyline_last );

#endif // FRECHET_DISTANCE_H
