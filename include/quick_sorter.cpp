// file quick_sorter.cpp

#include "quick_sorter.h"

#include <iterator>
#include <algorithm>
#include <cassert>

namespace ns_parametric_search {

  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  Quick_sorter< Traits, BidirectionalIter >::Quick_sorter
  ( process_base_t* parent,
    BidirectionalIter first, BidirectionalIter last )
  //-------------------------------------------------- 
    : process_base_t( parent ),     // initialize base
      first_( first )               // save iterator
  {
    input_size_ = std::distance( first, last );
    
    if( input_size_ >= 2 ){
      polynomials_.reserve( input_size_ );
      segments_.reserve( input_size_ );
      new_segments_.reserve( input_size_);

      std::copy( first, last, std::back_inserter( polynomials_ ) );
      std::random_shuffle( polynomials_.begin(), polynomials_.end() );

      comparison_results_ = std::vector< wrapper< bool > >
	( input_size_ );    

      segments_.push_back( std::pair< std::size_t, std::size_t >
			   ( 0, input_size_) );

      schedule( &Quick_sorter< Traits, BidirectionalIter >::do_comparisons );
      schedule( &Quick_sorter< Traits, BidirectionalIter >::do_partition );
    }
  }


  
  



  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  Quick_sorter< Traits, BidirectionalIter >::~Quick_sorter()
  //-------------------------------------------------- 
  {
    // copy the sorted polynomials into the input container
    if ( input_size_ >= 2 ){ // nothing to do for 0 or 1
      std::copy( polynomials_.begin(), polynomials_.begin() + input_size_,
		 first_ );
    }    
  }


  


  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Quick_sorter< Traits, BidirectionalIter >::do_comparisons()
  //-------------------------------------------------- 
  {
    typedef std::vector< 
      std::pair< std::size_t, std::size_t > >::iterator iter_t;

    for( iter_t iter = segments_.begin(); iter != segments_.end(); ++iter )
      do_segment_comparisons( *iter );
  }




  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Quick_sorter< Traits, BidirectionalIter >::do_segment_comparisons
  ( const std::pair< std::size_t, std::size_t >& segment )
  //-------------------------------------------------- 
  {
    assert( segment.first <= segment.second );

    for( std::size_t polynomial = segment.first; 
	 polynomial != segment.second; ++polynomial ){
      compare( polynomial, segment.first );
    }
  }





  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Quick_sorter< Traits, BidirectionalIter >::compare
  ( std::size_t polynomial, std::size_t pivot )
  //-------------------------------------------------- 
  {
    typedef typename Traits::scheduler_t scheduler_t;
    scheduler_t::instance()->inc_no_comparisons_total();

    // easy case
    if( polynomial == pivot )
      comparison_results_[ polynomial ] = false;

    
    // too bad; we have to do real work
    else{
      spawn( comparison_t::lazy_create_comparison
	     ( this, 
	       comparison_results_[ polynomial ],
	       polynomials_[ polynomial ],
	       polynomials_[ pivot ] ) );
    }
  }


  


  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  void
  Quick_sorter< Traits, BidirectionalIter >::do_partition()
  //-------------------------------------------------- 
  {
    typedef std::vector< 
      std::pair< std::size_t, std::size_t > >::iterator iter_t;

    for( iter_t iter = segments_.begin(); iter != segments_.end(); ++iter )
      if( iter->second - iter->first > 1 ){
	std::size_t split = do_segment_partition( iter->first, iter->second );
	if ( split == iter->first ){
	  ++split;
	}
	new_segments_.push_back( std::pair< std::size_t, std::size_t >
				 ( iter->first, split ) );
	new_segments_.push_back( std::pair< std::size_t, std::size_t >
				 ( split, iter->second ) );
      }


    segments_.clear();
    segments_.swap( new_segments_ );

    if( !segments_.empty() ){
      schedule( &Quick_sorter< Traits, BidirectionalIter >::do_comparisons );
      schedule( &Quick_sorter< Traits, BidirectionalIter >::do_partition );
    }
  }




  //-------------------------------------------------- 
  template< class Traits, class BidirectionalIter >
  std::size_t
  Quick_sorter< Traits, BidirectionalIter >::do_segment_partition
  ( std::size_t first, std::size_t last )
  //-------------------------------------------------- 
  {
    assert( last > first );
    --last;

    while( true ){
      for( ; 
	   first != last &&
	     comparison_results_[ first ];
	   ++first );
      for( ;
	   last != first &&
	     !comparison_results_[ last ];
	   --last );
      if( first < last ){
	std::swap( polynomials_[ first ], polynomials_[ last ] );
	std::swap( comparison_results_[ first ], 
		   comparison_results_[ last ] );
	
      }
      else
	return first;
    }
  }


} // namespace ns_parametric_search

