// file root.h

#ifndef ROOT_H
#define ROOT_H

#include "spawnable.h"
#include <memory>

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits >
  class Root : public Spawnable< Traits >
  //--------------------------------------------------
  {
  public: // type definitions
    typedef typename Traits::comparison_base_t  comparison_base_t;
    typedef typename Traits::number_t           number_t;

  public: // member functions
    Root( comparison_base_t* parent, const number_t& value );
    virtual ~Root();
    
    number_t value() const;

  private: // data members
    const number_t value_;
  };

} // namespace ns_parametric_search


#endif // ROOT_BASE_H
