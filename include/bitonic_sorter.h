// file bitonic_sorter.h

#ifndef BITONIC_SORTER_H
#define BITONIC_SORTER_H

#include <vector>
#include <cstddef>

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  class Bitonic_sorter : public Traits::process_base_t
  //--------------------------------------------------
  {
  public: // type definitions
    typedef typename Traits::process_base_t process_base_t;

  public: // member functions
    Bitonic_sorter( process_base_t* parent,
		    BidirectionalIter first,
		    BidirectionalIter last );
    ~Bitonic_sorter();

  private: // local type definitions
    typedef typename Traits::comparison_t comparison_t;
    typedef typename Traits::polynomial_t polynomial_t;

    enum e_direction { up, down };
    
    //-------------------------------------------------- 
    struct polynomial_wrapper 
    //-------------------------------------------------- 
    {
      polynomial_wrapper( const polynomial_t& polynomial, 
			  bool is_symbolic_max = false );

      polynomial_t polynomial_;
      bool   is_symbolic_max_;
    };



    //-------------------------------------------------- 
    struct wrap_polynomial 
    //-------------------------------------------------- 
    {
      wrap_polynomial( bool is_symbolic_max = false );
      polynomial_wrapper operator()( const polynomial_t& polynomial );

      bool is_symbolic_max_;
    };



    //-------------------------------------------------- 
    struct unwrap_polynomial 
    //-------------------------------------------------- 
    {
      polynomial_t operator()( const polynomial_wrapper& polynomial );
    };


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
    void pre_comparison_merge_phase();
    void post_comparison_merge_phase();
    void compare( std::size_t lower, std::size_t upper, 
		  std::size_t comparison_result_index );
    void test_and_exchange(std::size_t lower, std::size_t upper, 
			   std::size_t comparison_result_index,
			   e_direction direction );
    static void reverse( e_direction& direction );


  private: // data members
    BidirectionalIter first_;
    std::vector< polynomial_wrapper > polynomials_;
    std::vector< wrapper< bool > > comparison_results_;
    std::size_t input_size_;
    std::size_t internal_size_;
    std::size_t segment_size_;
    std::size_t sub_segment_size_;
    std::size_t comparator_offset_;
  };

} // namespace ns_parametric_search

#endif // BITONIC_SORTER_H



