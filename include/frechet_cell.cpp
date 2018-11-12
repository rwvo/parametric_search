// file frechet_cell.cpp

#include "frechet_cell.h"

#include <CGAL/squared_distance_2.h>

#include <cassert>
#include <cmath>


namespace { // declarations of constants and utility functions with file scope

  template< typename Traits > 
  typename Traits::number_t calc_squared_center
  ( const typename Frechet_cell_edge< Traits >::Segment& segment, 
    const typename Frechet_cell_edge< Traits >::Point& point );


  template< typename Traits > 
  typename Traits::number_t calc_center
  ( const typename Frechet_cell_edge< Traits >::Segment& segment, 
    const typename Frechet_cell_edge< Traits >::Point& point );

} // unnamed namespace




//-------------------------------------------------- 
// member functions of class Frechet_cell_edge

// constructor
//-------------------------------------------------- 
template< typename Traits >
Frechet_cell_edge< Traits >::Frechet_cell_edge
( const Segment& segment, const Point& point )
//-------------------------------------------------- 
  :  is_phantom_edge_( false ),
     squared_distance_( CGAL::squared_distance( segment.supporting_line(),
					        point ) ),
     squared_center_( calc_squared_center< Traits >
		      ( segment, point ) ),
     center_( calc_center< Traits >
	      ( segment, point ) ),
     squared_min_eps_( CGAL::squared_distance( segment, point ) ),
     length_( std::sqrt( static_cast< number_t >( segment.squared_length() ) ) )
{
  // EMPTY
}



// constructor for "phantom edges"
//-------------------------------------------------- 
template< typename Traits >
Frechet_cell_edge< Traits >::Frechet_cell_edge
( phantom_edge )
//-------------------------------------------------- 
  :  is_phantom_edge_( true )
{
  // EMPTY
}



//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::lower_bound() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return lower_bound_;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::upper_bound() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return upper_bound_;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::squared_distance() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return squared_distance_;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::distance() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return std::sqrt( static_cast< number_t >( squared_distance_ ) );
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::squared_center() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return squared_center_;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::center() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return center_;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::squared_min_eps() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return squared_min_eps_;
}




//-------------------------------------------------- 
template< typename Traits >
typename Traits::number_t
Frechet_cell_edge< Traits >::length() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return length_;
}




//-------------------------------------------------- 
template< typename Traits >
bool
Frechet_cell_edge< Traits >::is_empty() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return is_empty_;
}




//-------------------------------------------------- 
template< typename Traits >
bool
Frechet_cell_edge< Traits >::is_phantom_edge() const
//-------------------------------------------------- 
{
  return is_phantom_edge_;
}




//-------------------------------------------------- 
template< typename Traits >
bool
Frechet_cell_edge< Traits >::path_has_slack() const
//-------------------------------------------------- 
{
  assert( !is_phantom_edge_ );
  return path_has_slack_;
}




//-------------------------------------------------- 
template< typename Traits >
void
Frechet_cell_edge< Traits >::calc_ellips_intersections
( const number_t& squared_eps )
//-------------------------------------------------- 
{
  if( is_phantom_edge_ ){
    return;
  }

  if( squared_eps < squared_min_eps_ ){
    is_empty_ = true;
  }
  else { // eps >= min_eps_
    is_empty_ = false;
    number_t offset( std::sqrt( static_cast< number_t > 
				( squared_eps - squared_distance_ ) ) );

    lower_bound_ = 
      std::max( static_cast< number_t >( 0 ), center_ - offset );
    
    upper_bound_ = 
      std::min( static_cast< number_t >( length_ ), center_ + offset );

    if( lower_bound_ > upper_bound_ )
      is_empty_ = true;
  }
}




//-------------------------------------------------- 
template< typename Traits >
void
Frechet_cell_edge< Traits >::calc_monotone_reachable_bounds
( const Frechet_cell_edge< Traits >& parallel_edge,
  const Frechet_cell_edge< Traits >& perpendicular_edge )
//-------------------------------------------------- 

// Precondition: *this edge is not incident to the left or bottom
// boundary of the Frechet matrix. If *this is a left_edge, then the
// arguments should be edges of the cell to the left; if *this is a
// bottom_edge_, then the arguments should be edges of the cell below.

{
  if( is_phantom_edge() ){
    return;
  }

  if( perpendicular_edge.is_empty() ){
    if( parallel_edge.is_empty() ){
      is_empty_ = true;
    }
    else {
      if( parallel_edge.lower_bound() > lower_bound_ ){
	lower_bound_ = parallel_edge.lower_bound();

	if( lower_bound_ > upper_bound_ ){ // empty range
	  is_empty_ = true;
	}
      }
    }
  }


  // we can reach the cell bounded from the right or from above by
  // this edge through two edges. If there is no path with slack
  // (i.e., epsilon could be smaller, and the path would still exist)
  // through either of these edges, then the path to this cell/edge
  // has no slack either. On the other hand, this edge could be the
  // limiting factor itself: if the `passage' through this edge is
  // empty or a single point, then no path through this edge has slack.

  if( is_empty_
      || ( !parallel_edge.path_has_slack() && !perpendicular_edge.path_has_slack() )
      || lower_bound_ == upper_bound_ ){
    path_has_slack_ = false;
  }
  else {
    path_has_slack_ = true;
  }
}





//-------------------------------------------------- 
template< typename Traits >
void
Frechet_cell_edge< Traits >::calc_monotone_reachable_bounds
( const number_t& squared_eps,
  const Frechet_cell_edge< Traits >& previous_boundary_edge )
//--------------------------------------------------
// Precondition: *this edge is incident to either the left boundary of
// the Frechet matrix (i.e., it is the left_edge_ of a cell [0,j]), or
// it is incident to the bottom boundary of the Frechet matrix (i.e.,
// it is a bottom_edge_ of a cell [i,0]), but not both (i.e., no edge
// of [0, 0].

{
  if( is_phantom_edge() ){
    return;
  }


  // if we can reach the end of the previous boundary edge, we can
  // also reach the beginning of this boundary edge, and the monotone
  // reachable bounds are simply the elliptic intersections
  if( !previous_boundary_edge.is_empty() &&
      previous_boundary_edge.upper_bound() == 
      previous_boundary_edge.length() ){
    // this assertion may fail due to rounding errors
    // assert( normalized_lower_bound_ == 0 );

    // the upper_bound of the previous edge must not be a limiting
    // factor, and neither must the lower_bound of this one. But we
    // only have to test the latter: if we can decrease eps and still
    // reacht te beginning of this edge, we can also reach the end of
    // the previous edge

    path_has_slack_ = previous_boundary_edge.path_has_slack() &&
      squared_eps - squared_distance_ > squared_center_;
  }
  else {
    is_empty_ = true;
    path_has_slack_ = false;
  }
}





//-------------------------------------------------- 
template< typename Traits >
void
Frechet_cell_edge< Traits >::calc_monotone_reachable_bounds
( const number_t& squared_eps )
//-------------------------------------------------- 
// Precondition: *this edge is incident to the origin of the Frechet
// matrix, i.e., it is the left_edge_ or bottom_edge_ of cell [0, 0]
// in the Frechet matrix.

{
  if( is_phantom_edge() ){
    return;
  }

  if( lower_bound_ == 0 ){
    path_has_slack_ = 
      squared_eps - squared_distance_ > squared_center_;
  }
  else {
    is_empty_ = true;
    path_has_slack_ = false;
  }
}






//-------------------------------------------------- 
// member functions of class Frechet_cell


//-------------------------------------------------- 
template< typename Traits >
Frechet_cell< Traits >::Frechet_cell
( const Segment& left_segment, const Point& left_point,
  const Segment& bottom_segment, const Point& bottom_point )
//-------------------------------------------------- 
  : left_edge_( left_segment, left_point ),
    bottom_edge_( bottom_segment, bottom_point )
{
  // EMPTY
}



//-------------------------------------------------- 
template< typename Traits >
Frechet_cell< Traits >::Frechet_cell
( phantom_edge,
  const Segment& bottom_segment, const Point& bottom_point )
//-------------------------------------------------- 
  : left_edge_( phantom_edge() ),
    bottom_edge_( bottom_segment, bottom_point )
{
  // EMPTY
}



//-------------------------------------------------- 
template< typename Traits >
Frechet_cell< Traits >::Frechet_cell
( const Segment& left_segment, const Point& left_point,
  phantom_edge )
//-------------------------------------------------- 
  : left_edge_( left_segment, left_point ),
    bottom_edge_( phantom_edge() )
{
  // EMPTY
}



//-------------------------------------------------- 
template< typename Traits >
Frechet_cell< Traits >::Frechet_cell
( phantom_edge, phantom_edge )
//-------------------------------------------------- 
  : left_edge_( phantom_edge() ),
    bottom_edge_( phantom_edge() )
{
  // EMPTY
}



//-------------------------------------------------- 
template< typename Traits >
const Frechet_cell_edge< Traits >&
Frechet_cell< Traits >::left_edge() const
//-------------------------------------------------- 
{
  return left_edge_;
}




//-------------------------------------------------- 
template< typename Traits >
const Frechet_cell_edge< Traits >&
Frechet_cell< Traits >::bottom_edge() const
//-------------------------------------------------- 
{
  return bottom_edge_;
}




//-------------------------------------------------- 
template< typename Traits >
void
Frechet_cell< Traits >::calc_ellips_intersections
( const number_t& squared_eps )
//-------------------------------------------------- 
{
  // third argument to next two calls indicates `left edge'
  left_edge_.calc_ellips_intersections( squared_eps );
  bottom_edge_.calc_ellips_intersections( squared_eps );
}



//-------------------------------------------------- 
template< typename Traits >
void
Frechet_cell< Traits >::calc_monotone_reachable_bounds
( const number_t& squared_eps, 
  Frechet_cell* left, Frechet_cell* below )
//-------------------------------------------------- 
{
  // case 1: cell[ 0, 0 ] (starting cell).
  // case 2: cell[ 0, j ] (cell left column)
  // case 3: cell[ i, 0 ] (cell in bottom row)
  // case 4: cell[ i, j ] (general cell)
  //

  // we call three overloads of calc_monotone_reachable_bounds: one
  // for the two edges incident to the origin, one for edges incident
  // to the left or bottom of the matrix (other than the two edges of
  // the previous case), and one for the remaining edges.



  // case 1: both edges are incident to the origin
  if( !left && !below ){
    left_edge_.calc_monotone_reachable_bounds( squared_eps );
    bottom_edge_.calc_monotone_reachable_bounds( squared_eps );
  }

  // case 2: the bottom edge is `standard', but the left
  // edge is a boundary edge, and needs special treatment
  else if( !left && below ){
    left_edge_.calc_monotone_reachable_bounds
      ( squared_eps, below->left_edge_ );
    bottom_edge_.calc_monotone_reachable_bounds
      ( below->bottom_edge_, below->left_edge_);
  }

  // case 3: the left edge is `standard', but the bottom
  // edge is a boundary edge, and needs special treatment
  else if( left && !below ){
    left_edge_.calc_monotone_reachable_bounds
      ( left->left_edge_, left->bottom_edge_ );
    bottom_edge_.calc_monotone_reachable_bounds
      ( squared_eps, left->bottom_edge_ );
  }

  // case 4: your average `internal' cell.
  else {
    assert( left && below );
    left_edge_.calc_monotone_reachable_bounds
      ( left->left_edge_, left->bottom_edge_ );
    bottom_edge_.calc_monotone_reachable_bounds
      ( below->bottom_edge_, below->left_edge_);
  }
}







//-------------------------------------------------- 

namespace { // definitions of utility functions with file scope

  //-------------------------------------------------- 
  template< typename Traits >
  typename Traits::number_t calc_squared_center
  ( const typename Frechet_cell_edge< Traits >::Segment& segment, 
    const typename Frechet_cell_edge< Traits >::Point& point )
    //-------------------------------------------------- 
  {
    typedef typename Frechet_cell_edge< Traits >::Segment Segment;
    typedef typename Frechet_cell_edge< Traits >::Point   Point;

    assert( segment.squared_length() > 0 );

    // calculate the projection of `point' on the supporting line of
    // `segment'
    Point projected_point = 
      segment.supporting_line().projection( point );
  
    return CGAL::squared_distance( segment.source(), 
				   projected_point ); 
  }





  //-------------------------------------------------- 
  template< typename Traits >
  typename Traits::number_t calc_center
  ( const typename Frechet_cell_edge< Traits >::Segment& segment, 
    const typename Frechet_cell_edge< Traits >::Point& point )
    //-------------------------------------------------- 
  {
    typedef typename Frechet_cell_edge< Traits >::Segment Segment;
    typedef typename Frechet_cell_edge< Traits >::Point   Point;

    assert( segment.squared_length() > 0 );

    // calculate the projection of `point' on the supporting line of
    // `segment'
    Point projected_point = 
      segment.supporting_line().projection( point );
  
    // the source of the segment is mapped to 0. The value of the
    // projected point is the distance to the source. If this
    // projected point is `to the left' of the source, the distance
    // should be negative.
    int sign = 1;
    if( Segment( projected_point, 
		 segment.target() ).collinear_has_on( segment.source() ) )
      sign = -1;

    typedef typename Frechet_cell_edge< Traits >::number_t number_t;
    return sign * std::sqrt( 
	static_cast< number_t >
	( CGAL::squared_distance( segment.source(), projected_point ) ) ); 
  }

} // unnamed namespace
