// file median_of_lines.cpp

// This program solves the `median-of-lines problem' using parametric
// search. See the documentation/tutorial


#include "line_traits.h"
#include "quick_sorter.h"
#include <iostream>
#include <vector>

//-------------------------------------------------- 
void 
generate_increasing_lines
( std::vector< Line_traits::polynomial_t>& lines, 
  std::vector< Line_traits::polynomial_t>::size_type number_of_lines );
//-------------------------------------------------- 




int main(){
  //-------------------------------------------------- 
  // create a collection of monotone increasing linear function, i.e.,
  // lines with positive slope

  typedef Line_traits::number_t     number_t;
  typedef Line_traits::polynomial_t line_t;

  std::vector< line_t > lines;
  const std::vector< line_t >::size_type number_of_lines( 101 );

  lines.reserve( number_of_lines );
  generate_increasing_lines( lines, number_of_lines );
  //-------------------------------------------------- 







  //-------------------------------------------------- 
  // declare a decision problem solver. It must be a dynamic object
  // (i.e., created with `new'), and it will be owned by the
  // framework (we must not delete it ourselves).

  typedef Line_solver< Line_traits > solver_t;

  new solver_t( lines.begin(), lines.end() );
  //-------------------------------------------------- 







  //-------------------------------------------------- 
  // create a sorter, and use it to sort the lines at the unknown
  // value lambda-star.

  typedef std::vector< line_t >::iterator iter_t;
  typedef pms::Quick_sorter< Line_traits, iter_t > sorter_t;

  // Create a sorter. Upon creation, it will register itself with the
  // parametric search framework. the framework takes ownership, and
  // takes care of its destruction after it has finished. The first
  // argument to the sorter is its parent process object. Since it is
  // the main process, it has no parent.

  new sorter_t( 0, lines.begin(), lines.end() );

  // tell the framework to start the main process

  pms::Scheduler< Line_traits >::instance()->run();
  //-------------------------------------------------- 







  //-------------------------------------------------- 

  // At this point, the scheduler has finished. We retrieve the median
  // line, and calculate its intersection with the x-axis. This is the
  // sought-after value lambda-star.

  // since we have sorted the lines by y-coordinate at the value
  // lambda-star, the median line at lambda-star is the middle one

  line_t median_line = 
    lines[ lines.size() / 2 ];
    
  // compute intersection with the x-axis of line y = ax + b

  assert( median_line.a != 0 );
  number_t lambda_star = - median_line.b / median_line.a;

  std::cout << "Lambda-star = " << lambda_star << std::endl;

  //-------------------------------------------------- 
}




//-------------------------------------------------- 
void 
generate_increasing_lines
( std::vector< Line_traits::polynomial_t>& lines, 
  std::vector< Line_traits::polynomial_t>::size_type number_of_lines )
//-------------------------------------------------- 
{
  typedef Line_traits::polynomial_t                          line_t;
  typedef std::vector< Line_traits::polynomial_t>::size_type size_t;
  
  // Generate a bunch of lines with *positive* slope. You may want to
  // replace the arbitrary way of generating lines by something more
  // meaningful (read lines from std::cin, for instance, or more
  // random.

  int a(1), b(0);
  for( size_t i = 0; i< number_of_lines; ++i ){
    a = (a*i) % number_of_lines + 1;
    b = (b*i) % number_of_lines - a;
    lines.push_back( line_t( a, b ) );
  }
}
