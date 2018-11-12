// file line_traits.h

#ifndef LINE_TRAITS_H
#define LINE_TRAITS_H

// include standard components of the framework
#include "traits_base.h"

// include our own problem-specific classes
#include "line.h"
#include "line_comp.h"
#include "line_solver.h"

// define the traits class simply by deriving from Sorting_traits
struct Line_traits : 
  public Sorting_traits< Line_traits, double,
                         Line, Line_comp, Line_solver > {};

#endif // LINE_TRAITS_H
