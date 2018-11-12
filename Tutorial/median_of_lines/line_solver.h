// file line_solver.h

#ifndef LINE_SOLVER_H
#define LINE_SOLVER_H

#include "line.h"
#include <vector>

//--------------------------------------------------
template< class Traits >
class Line_solver : public Traits::solver_base_t
//--------------------------------------------------
{
public: // type declarations
  typedef typename Traits::number_t           number_t;
  typedef typename Traits::polynomial_t       polynomial_t;
  typedef typename Traits::solver_base_t      solver_base_t;
  typedef typename solver_base_t::decision_t  decision_t;

public: // member functions
  template< typename input_iter_t >
  Line_solver( const input_iter_t& first, const input_iter_t& last );

  virtual decision_t decide( const number_t& x_coord );

private: // type declarations
  class Line_comparator;

private: // member functions
  Line_solver(); // surpress generation of *public* default
                 // constructor by compiler

private: // data members
  std::vector< polynomial_t > lines_;
};



// definition of template constructor
//-------------------------------------------------- 
template< class Traits >
template< typename input_iter_t >
Line_solver< Traits >::Line_solver
( const input_iter_t& first, const input_iter_t& last )
//-------------------------------------------------- 
  : lines_( first, last )
{
  // EMPTY
}


#endif // LINE_SOLVER_H
