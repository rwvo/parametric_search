// file quick_sorter.h

#ifndef QUICK_SORTER_H
#define QUICK_SORTER_H

#include <vector>
#include <list>
#include <utility>
#include <cstddef>

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  class Quick_sorter : public Traits::process_base_t
  //--------------------------------------------------
  {
  public: // type definitions
    typedef typename Traits::process_base_t process_base_t;

  public: // member functions
    Quick_sorter( process_base_t* parent,
		  BidirectionalIter first,
		  BidirectionalIter last );
    ~Quick_sorter();

  private: // local type definitions
    typedef typename Traits::comparison_t comparison_t;
    typedef typename Traits::polynomial_t polynomial_t;


    //-------------------------------------------------- 
    template< typename T >
    struct wrapper
    //-------------------------------------------------- 
    {
      wrapper() : t_() {}
      wrapper( const T& t ) : t_( t ) {}
      operator T&() { return t_; }
      T t_;
    };

  private: // member functions
    void do_comparisons();
    void do_segment_comparisons
    ( const std::pair< std::size_t, std::size_t >& segment );
    void compare( std::size_t polynomial, std::size_t pivot );
    void do_partition();
    std::size_t do_segment_partition
    ( std::size_t first, std::size_t last );
    
  private: // data members
    BidirectionalIter first_;
    std::vector< polynomial_t > polynomials_;
    std::vector< wrapper< bool > > comparison_results_;
    std::vector< std::pair< std::size_t, std::size_t > > segments_;
    std::vector< std::pair< std::size_t, std::size_t > > new_segments_;
    std::size_t input_size_;
  };

} // namespace ns_parametric_search

#endif // QUICK_SORTER_H



