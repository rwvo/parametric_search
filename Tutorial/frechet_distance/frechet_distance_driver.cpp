#include "frechet_traits.h"
#include "frechet_distance.h"

#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>

#include <CGAL/Random.h>

typedef Frechet_traits::Point Point;
typedef std::vector< Point >  Polyline;

Polyline random_polyline( std::size_t no_points, 
                          double x_lower, double x_upper,
                          double y_lower, double y_upper );

void stats( const Polyline& p, const Polyline& q );

//-------------------------------------------------- 
int main()
//-------------------------------------------------- 
{
  // create two polylines with 80 vertices each, and with all vertices
  // in the square [0,1] x [0,1]

  Polyline p( random_polyline( 80, 0, 1, 0, 1 ) );
  Polyline q( random_polyline( 80, 0, 1, 0, 1 ) );
   
  // compute the frechet distance between the polylines
  frechet_distance< Frechet_traits >( p.begin(), p.end(), 
				      q.begin(), q.end() );

  // look at the results, and print some statistics
  stats( p, q );
}





//-------------------------------------------------- 
Polyline random_polyline( std::size_t no_points, 
                          double x_lower, double x_upper,
                          double y_lower, double y_upper )
//--------------------------------------------------                          
{
  static CGAL::Random random( std::time( 0 ) );
  Polyline pl;
  for( std::size_t i = 0; i != no_points; ++i ){
    pl.push_back( Point( random.get_double( x_lower, x_upper ), 
                         random.get_double( y_lower, y_upper ) ) );
  }

  return pl;
}




//-------------------------------------------------- 
void stats( const Polyline& p, const Polyline& q )
//-------------------------------------------------- 
{
  
  std::cout  << std::setprecision(40);
  std::cout << "Lower bound:                        "
	    << std::sqrt( pms::Scheduler< Frechet_traits >::instance()
			  ->lower_bound() )
	    << "\n"
    
	    << "Upper bound:                        "
	    << std::sqrt( pms::Scheduler< Frechet_traits >::instance()
			  ->upper_bound() )
	    << "\n"
    
	    << "Input size:                         "
	    << p.size() << " + " << q.size() << " = "
	    << p.size() + q.size()
	    << "\n"
	 
	    << "Number of processes:                "
	    <<  pms::Scheduler< Frechet_traits >::instance()->no_processes()
	    <<"\n"
	 
	    << "Number of comparisons (total):      "
	    <<  pms::Scheduler< Frechet_traits >::instance()
                                ->no_comparisons_total()
	    <<"\n"
	 
	    << "Number of comparisons (actual):     "
	    <<  pms::Scheduler< Frechet_traits >::instance()->
                                                  no_comparisons()
	    <<"\n"
	 
	    << "Number of roots:                    "
	    <<  pms::Scheduler< Frechet_traits >::instance()->no_roots()
	    <<"\n"
	 
	    << "Number of (expensive) computations: "
	    <<  pms::Scheduler< Frechet_traits >::instance()->no_computations()
	    <<"\n";
}






