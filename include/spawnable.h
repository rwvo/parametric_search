// file spawnable.h

#include <cstddef>

#ifndef SPAWNABLE_H
#define SPAWNABLE_H

// Implementers of derived classes of Process_base, Comparison_base,
// and Root_base need not look here; check "process_base.h",
// "comparison_base.h", and "root_base.h" instead.

// class Spawnable is a common base class of Process_base,
// Comparison_base, and Root_base. These classes need to notify their
// spawning process (i.e., the process--derived from Spawner--that
// spawned/created/called then) when they are finished. The
// constructor of this common base class takes and stores a pointer to
// the spawning process; the destructor takes care of notifying the
// spawning process.

// #include "LOKI/SmallObj.h"

namespace ns_parametric_search {

  template< class Traits > class spawner;

  //--------------------------------------------------
  template< class Traits >
  class Spawnable // : public Loki::SmallObject<>
  //--------------------------------------------------
  {
  public: // member functions
    Spawnable( Spawner< Traits >* parent );
    virtual ~Spawnable() = 0;
    
    // static void stats();

  private: // data members
    Spawner< Traits >* parent_; // not owned

    // static std::size_t created;
    // static std::size_t destroyed;
  };

} // namespace ns_parametric_search


#endif // SPAWNABLE_H
