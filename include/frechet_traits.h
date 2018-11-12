// file frechet_traits.h

#ifndef FRECHET_TRAITS_H
#define FRECHET_TRAITS_H

// include standard components of the framework
#include "traits_base.h"

// include our own problem-specific classes
#include "frechet_polynomial.h"
#include "frechet_comp.h"
#include "frechet_solver.h"

#include "cgal_kernel.h"
#include <CGAL/Point_2.h>

// define the traits class simply by deriving from Sorting_traits
// and specifying the desired point type

struct Frechet_traits : 
  public Sorting_traits< Frechet_traits, long double,
                         Frechet_polynomial, Frechet_comp, Frechet_solver > 
{
  typedef CGAL::Cartesian< double >        CGAL_kernel_t;
  typedef CGAL::Point_2< CGAL_kernel_t >   Point;
};

#endif // FRECHET_TRAITS_H
