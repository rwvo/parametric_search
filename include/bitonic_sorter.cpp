// file bitonic_sorter.cpp

#include "bitonic_sorter.h"

#include <iterator>
#include <algorithm>
#include <cassert>


namespace ns_parametric_search {

  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  Bitonic_sorter< Traits, BidirectionalIter >::Bitonic_sorter
  ( process_base_t* parent,
    BidirectionalIter first, BidirectionalIter last )
  //-------------------------------------------------- 
    : process_base_t( parent ),     // initialize base
      first_( first ),              // save iterators 
      segment_size_( 2 ),
      sub_segment_size_( 2 ),
      comparator_offset_( 1 )
  {
    input_size_ = std::distance( first, last );

    if ( input_size_ >= 2 ){ // nothing to do for 0 or 1
      internal_size_ = next_greater_or_equal_power_of_two( input_size_ );

      assert( internal_size_ <= polynomials_.max_size() );
      polynomials_.reserve( internal_size_ );

      std::transform( first, last, 
		      std::back_inserter( polynomials_ ), wrap_polynomial() );
    
      // copy an arbitrary input polynomial, make it a symbolic maximum,
      // and fill the remaining part of our internal vector with
      // copies of this symbolic maximum.
      polynomial_wrapper symbolic_max( *first, true );
      polynomials_.resize( internal_size_, symbolic_max );

      // the number of comparisons in eacht iteration of the sorter is
      // half the number of polynomials that we sort.
      comparison_results_ = std::vector< wrapper< bool > >
      	( internal_size_ / 2 );    

      schedule( &Bitonic_sorter::pre_comparison_merge_phase );
      schedule( &Bitonic_sorter::post_comparison_merge_phase );
    }

  }


  



  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  Bitonic_sorter< Traits, BidirectionalIter >::~Bitonic_sorter()
  //-------------------------------------------------- 
  {
    // copy the sorted polynomials into the input container
    if ( input_size_ >= 2 ){ // nothing to do for 0 or 1
      std::transform( polynomials_.begin(), 
		      polynomials_.begin() + input_size_,
		      first_, unwrap_polynomial() );
    }    
  }



  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Bitonic_sorter< Traits, BidirectionalIter >::pre_comparison_merge_phase()
  //-------------------------------------------------- 
  {
    std::size_t comparison_result_index = 0;

    // for all segments
    for( std::size_t segment_start = 0; segment_start < internal_size_; 
	 segment_start += segment_size_ ){

      // for all subsegments in the segment
      for( std::size_t sub_segment_start = segment_start;
	   sub_segment_start < segment_start + segment_size_;
	   sub_segment_start += sub_segment_size_ ){

	// for all comparators in the subsegment
	for( std::size_t lower = sub_segment_start, 
	       upper = lower + comparator_offset_;
	     lower < sub_segment_start + ( sub_segment_size_ / 2 );
	     ++lower, ++upper, ++comparison_result_index ){
	  
	  compare( lower, upper, comparison_result_index );
	}
      }
    }
  }



  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Bitonic_sorter< Traits, BidirectionalIter >::post_comparison_merge_phase()
  //-------------------------------------------------- 
  {
    e_direction direction = up;
    std::size_t comparison_result_index = 0;

    // for all segments
    for( std::size_t segment_start = 0; segment_start < internal_size_; 
	 segment_start += segment_size_ ){

      // for all subsegments in the segment
      for( std::size_t sub_segment_start = segment_start;
	   sub_segment_start < segment_start + segment_size_;
	   sub_segment_start += sub_segment_size_ ){

	// for all comparators in the subsegment
	for( std::size_t lower = sub_segment_start, 
	       upper = lower + comparator_offset_;
	     lower < sub_segment_start + ( sub_segment_size_ / 2 );
	     ++lower, ++upper, ++comparison_result_index ){
	  
	  test_and_exchange( lower, upper, comparison_result_index,
			     direction );
	}
      }

      reverse( direction );
    }

    // prepare for the next iteration, or decide that we are done
    bool done( false );

    if( comparator_offset_ == 1 ){
      if( segment_size_ == internal_size_ ){
	done = true;
      }
      else{
	segment_size_ *= 2;
	sub_segment_size_ = segment_size_;
	comparator_offset_ = segment_size_ / 2;
      }
    }
    else{
      sub_segment_size_ /= 2;
      comparator_offset_ /= 2;
    }

    if( !done ){
      schedule( &Bitonic_sorter::pre_comparison_merge_phase );
      schedule( &Bitonic_sorter::post_comparison_merge_phase );
    }
  }




  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Bitonic_sorter< Traits, BidirectionalIter >::compare
  ( std::size_t lower, std::size_t upper, std::size_t comparison_result_index )
  //-------------------------------------------------- 
  {
    typedef typename Traits::scheduler_t scheduler_t;
    scheduler_t::instance()->inc_no_comparisons_total();

    // first easy case:
    if ( polynomials_[ lower ].is_symbolic_max_ )
      comparison_results_[ comparison_result_index ] = false;

    // second easy case:
    else if ( polynomials_[ upper ].is_symbolic_max_ )
      comparison_results_[ comparison_result_index ] = true;


    // too bad; we have to do real work
    else{
      spawn( comparison_t::lazy_create_comparison
	     ( this,
	       comparison_results_[ comparison_result_index ],
	       polynomials_[ lower ].polynomial_,
	       polynomials_[ upper ].polynomial_ ) );
    }
  }





  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Bitonic_sorter< Traits, BidirectionalIter >::test_and_exchange
  ( std::size_t lower, std::size_t upper, 
    std::size_t comparison_result_index, e_direction direction )
  //-------------------------------------------------- 
  {
    if( ( comparison_results_[ comparison_result_index ] &&
	  direction == down )
	||
	( !comparison_results_[ comparison_result_index ] &&
	  direction == up ) )

      std::swap( polynomials_[ lower ], polynomials_[ upper ] );
  }





  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void 
  Bitonic_sorter< Traits, BidirectionalIter >::reverse
  ( e_direction& direction )
  //-------------------------------------------------- 
  {
    direction = static_cast< e_direction >( 1 - direction );
  }




  //-------------------------------------------------- 
  // Definitions of members of local classes

  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  Bitonic_sorter< Traits, BidirectionalIter >::
  polynomial_wrapper::polynomial_wrapper
  ( const polynomial_t& polynomial, bool is_symbolic_max )
  //-------------------------------------------------- 
    : polynomial_( polynomial ),
      is_symbolic_max_( is_symbolic_max )
  {
    // EMPTY
  }




  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  Bitonic_sorter< Traits, BidirectionalIter >::
  wrap_polynomial::wrap_polynomial
  ( bool is_symbolic_max )
  //-------------------------------------------------- 
    : is_symbolic_max_( is_symbolic_max )
  {
    // EMPTY
  }



  
  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  typename Bitonic_sorter< Traits, BidirectionalIter >::polynomial_wrapper
  Bitonic_sorter< Traits, BidirectionalIter >::wrap_polynomial::operator()
  ( const polynomial_t& polynomial )
  //-------------------------------------------------- 
  {
    return polynomial_wrapper( polynomial, is_symbolic_max_ );
  }



  
  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  typename Bitonic_sorter< Traits, BidirectionalIter >::polynomial_t
  Bitonic_sorter< Traits, BidirectionalIter >::unwrap_polynomial::operator()
  ( const polynomial_wrapper& polynomial )
  //-------------------------------------------------- 
  {
    return polynomial.polynomial_;
  }



  
  //-------------------------------------------------- 
  namespace { // utilities with file scope


    //-------------------------------------------------- 
    std::size_t 
    next_greater_or_equal_power_of_two( std::size_t size )
    //-------------------------------------------------- 
    {
      // this function returns the smallest power of two that is
      // greater than or equal to the input

      if( size == 0 ) 
	return 0;

      std::size_t power_of_two = 1;
      while( power_of_two > 0 && power_of_two < size )
	power_of_two *= 2;
    
      assert( power_of_two > 0 ); // check for overflow
      return power_of_two;
    }



  } // unnamed namespace
  
} // namespace ns_parametric_search

