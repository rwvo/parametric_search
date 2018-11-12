// file traits_base.h

#ifndef TRAITS_BASE_H
#define TRAITS_BASE_H

// include standard components of the framework
#include "scheduler.h"
#include "process_base.h"
#include "comparison_base.h"
#include "root.h"
#include "solver_base.h"

// define a shorthand for the framework namespace
namespace pms = ns_parametric_search;


//-------------------------------------------------- 
template< class Traits >
struct Traits_base
//--------------------------------------------------
{
  typedef pms::Scheduler< Traits >       scheduler_t;
  typedef pms::Process_base< Traits >    process_base_t;
  typedef pms::Comparison_base< Traits > comparison_base_t;
  typedef pms::Root< Traits >            root_t;
  typedef pms::Solver_base< Traits >     solver_base_t;
};


//-------------------------------------------------- 
template< class Traits, class Number,
          template< class > class Polynomial,
          template< class > class Comparison,
          template< class > class Solver >
struct Sorting_traits
//--------------------------------------------------
{
  typedef pms::Scheduler< Traits >       scheduler_t;
  typedef pms::Process_base< Traits >    process_base_t;
  typedef pms::Comparison_base< Traits > comparison_base_t;
  typedef pms::Root< Traits >            root_t;
  typedef pms::Solver_base< Traits >     solver_base_t;

  typedef Number                         number_t;
  typedef Polynomial< Traits >           polynomial_t;
  typedef Comparison< Traits >           comparison_t;
  typedef Solver< Traits >               decision_solver_t;
};


#endif // TRAITS_BASE_H
