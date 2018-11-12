// file delete_object.h

#ifndef DELETE_OBJECT_H
#define DELETE_OBJECT_H

namespace ns_parametric_search {

  // Function object class, to be used to call delete on all pointers
  // stored in a container

  //--------------------------------------------------
  class Delete_object
  //--------------------------------------------------
  {
  public:
    template< typename T >
    void operator()( T* p) { delete p; }
  };

} // namespace ns_parametric_search


#endif // DELETE_OBJECT_H
