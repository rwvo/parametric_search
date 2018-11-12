// file comparison_base.h

#ifndef COMPARISON_BASE_H
#define COMPARISON_BASE_H

#include "spawner.h"
#include "spawnable.h"

#include <memory>

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits >
  class Comparison_base : public Spawner< Traits >,
			  public Spawnable< Traits >
  //--------------------------------------------------
  {
  public: // type definitions
    typedef typename Traits::scheduler_t            scheduler_t;
    typedef typename Traits::process_base_t         process_base_t;
    typedef typename scheduler_t::comparison_iter_t comparison_iter_t;
    typedef typename Traits::number_t                number_t;

  public: // member functions
    // the process that creates us must pass us a pointer to itself, so
    // that we can notify our creator when we are finished.

    Comparison_base( process_base_t* parent );
    virtual ~Comparison_base() = 0;

  protected:
    void nothing_spawned();

  private: // member functions
    // make copy constructor and assignment operator inaccessible
    // both are not implemented

    Comparison_base( const Comparison_base& );
    Comparison_base& operator=( const Comparison_base& );

    virtual void children_have_finished();

    typedef typename Traits::scheduler_t           scheduler_t;
    friend scheduler_t;

    virtual void deduce( const number_t& lower_bound, 
			 const number_t& upper_bound) = 0;
  private: // data members
    comparison_iter_t comparison_iter_;
  };

} // namespace ns_parametric_search


#endif // COMPARISON_BASE_H
