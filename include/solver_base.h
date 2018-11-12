// file solver_base.h

#ifndef SOLVER_BASE_H
#define SOLVER_BASE_H

namespace ns_parametric_search {


  //--------------------------------------------------
  template< class Traits >
  class Solver_base 
  //--------------------------------------------------
  {
  public: // type definitions
    enum decision_t { less, equal, greater };
    typedef typename Traits::number_t number_t;
    
  public: // member functions
    Solver_base();
    virtual ~Solver_base();

    virtual decision_t decide( const number_t& ) = 0;

  };

} // namespace ns_parametric_search


#endif // SOLVER_BASE_H
