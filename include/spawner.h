// file spawner.h

#ifndef SPAWNER_H
#define SPAWNER_H

#include <cstddef>

namespace ns_parametric_search {

  template< class Traits > class Spawnable;

  //--------------------------------------------------
  template< class Traits >
  class Spawner
  //--------------------------------------------------
  {
  public:
    Spawner();
    virtual ~Spawner() = 0;
    std::size_t active_child_count() const;

  protected:
    void spawn( Spawnable< Traits >* child );

  private:
    friend class Spawnable< Traits >;
    void child_has_finished();
    virtual void children_have_finished() = 0;

  private:
    std::size_t active_child_count_;

  };

} // namespace ns_parametric_search



#endif // SPAWNER_H
