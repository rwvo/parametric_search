// file frechet_cell.h

#ifndef FRECHET_CELL_H
#define FRECHET_CELL_H

#include <cstddef>

#include "cgal_kernel.h"
#include <CGAL/Point_2.h>
#include <CGAL/Segment_2.h>

//-------------------------------------------------- 
template< typename Traits >
class Frechet_cell_edge
//-------------------------------------------------- 
{
public: // nested types
  typedef typename Traits::number_t                 number_t;
  typedef typename Traits::CGAL_kernel_t            CGAL_kernel_t;
  typedef typename CGAL::Segment_2< CGAL_kernel_t > Segment;
  typedef typename Traits::Point                    Point;
  
  struct phantom_edge {};
 
public: // member functions
  // constructor
  Frechet_cell_edge( const Segment& segment,
		     const Point& point );
  Frechet_cell_edge( phantom_edge ); // used for "phantom edges" of cells
                                     // [i+1, k] and [k, j+1]

  number_t lower_bound() const;
  number_t upper_bound() const;
  number_t squared_distance() const;
  number_t distance() const;
  number_t squared_center() const;
  number_t center() const;
  number_t squared_min_eps() const;
  number_t length() const;

  bool is_empty() const;
  bool is_phantom_edge() const;
  bool path_has_slack() const;

  void calc_ellips_intersections( const number_t& squared_eps );

  void calc_monotone_reachable_bounds
  ( const Frechet_cell_edge< Traits >& parallel_edge,
    const Frechet_cell_edge< Traits >& perpendicular_edge );
  // Precondition: *this edge is not incident to the left or bottom
  // boundary of the Frechet matrix. If *this is a left_edge, then the
  // arguments should be edges of the cell to the left; if *this is a
  // bottom_edge_, then the arguments should be edges of the cell
  // below.

  void calc_monotone_reachable_bounds
  ( const number_t& eps,
    const Frechet_cell_edge< Traits >& previous_boundary_edge );
  // Precondition: *this edge is incident to either the left boundary
  // of the Frechet matrix (i.e., it is the left_edge_ of a cell
  // [0,j]), or it is incident to the bottom boundary of the Frechet
  // matrix (i.e., it is a bottom_edge_ of a cell [i,0]), but not both
  // (i.e., no edge of [0, 0].


  void calc_monotone_reachable_bounds
  ( const number_t& squared_eps );
  // Precondition: *this edge is incident to the origin of the Frechet
  // matrix, i.e., it is the left_edge_ or bottom_edge_ of cell [0, 0]
  // in the Frechet matrix.


private: // data members
  bool is_phantom_edge_;

  number_t squared_distance_;
  number_t squared_center_;
  number_t center_;

  number_t squared_min_eps_;
  number_t length_;

  number_t lower_bound_;
  number_t upper_bound_;
  bool is_empty_;

  bool path_has_slack_;
};




//-------------------------------------------------- 
template< typename Traits >
class Frechet_cell
//-------------------------------------------------- 
{
public:
  typedef typename Traits::number_t                            number_t;
  typedef typename Frechet_cell_edge< Traits >::Segment        Segment;
  typedef typename Frechet_cell_edge< Traits >::Point          Point;
  typedef typename Frechet_cell_edge< Traits >::phantom_edge   phantom_edge;

public:
  Frechet_cell( const Segment& left_segment, const Point& left_point,
		const Segment& bottom_segment, const Point& bottom_point );
  Frechet_cell( phantom_edge,
		const Segment& bottom_segment, const Point& bottom_point );
  Frechet_cell( const Segment& left_segment, const Point& left_point,
		phantom_edge);
  Frechet_cell( phantom_edge, phantom_edge );


  const Frechet_cell_edge< Traits >& left_edge() const;
  const Frechet_cell_edge< Traits >& bottom_edge() const;

  void calc_ellips_intersections( const number_t& squared_eps );
  void calc_monotone_reachable_bounds( const number_t& squared_eps,
				       Frechet_cell* left,
				       Frechet_cell* below );


private:
  Frechet_cell_edge< Traits > left_edge_;
  Frechet_cell_edge< Traits > bottom_edge_;
};

#endif // FRECHET_CELL_H
