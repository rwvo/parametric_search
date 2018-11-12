// file root.cpp

#include "root.h"

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits >
  Root< Traits >::Root( comparison_base_t* parent,
			const number_t& value )
  //--------------------------------------------------
    : Spawnable< Traits >( parent ),
      value_( value )
  {
    typedef typename Traits::scheduler_t scheduler_t;
    scheduler_t::instance()->schedule( this );
  }




  //--------------------------------------------------
  template< class Traits >
  Root< Traits >::~Root()
  //--------------------------------------------------
  {
    // EMPTY
  }




  //--------------------------------------------------
  template< class Traits >
  typename Root< Traits >::number_t
  Root< Traits >::value() const
  //--------------------------------------------------
  {
    return value_;
  }


} // namespace ns_parametric_search

