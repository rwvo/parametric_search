// file frechet_preprocessor.cpp

#include "frechet_preprocessor.h"

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  Frechet_preprocessor< Traits, BidirectionalIter >::
  Frechet_preprocessor( process_base_t* parent,
			BidirectionalIter first, BidirectionalIter last )
  //-------------------------------------------------- 
    : process_base_t( parent ),    // initialize base
    first_( first ),               // save iterators
    last_( last )
  {
    this->schedule( &Frechet_preprocessor< Traits, BidirectionalIter >::
		    preprocess );
  }    

  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  Frechet_preprocessor< Traits, BidirectionalIter >::
  ~Frechet_preprocessor()
  //-------------------------------------------------- 
  {
    // EMPTY
  }    


  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  void
  Frechet_preprocessor< Traits, BidirectionalIter >::
  preprocess()
  //-------------------------------------------------- 
  {
    this->spawn( new Frechet_precomp< Traits, BidirectionalIter >
		 ( this, first_, last_ ) );
  }    



  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  Frechet_precomp< Traits, BidirectionalIter >::
  Frechet_precomp( process_base_t* parent,
		   BidirectionalIter first, BidirectionalIter last )
  //-------------------------------------------------- 
    : comparison_base_t( parent ) // initialize base
  {
    for( BidirectionalIter iter = first; iter != last; ++iter )
      // There are two polynomials per edge, sharing the same
      // squared_min_eps. We only need to treat one of them.
      if( iter->is_lower_bound_polynomial ){
	typedef typename Traits::root_t root_t;
	this->spawn( new root_t( this,  iter->squared_min_eps ) );
      }
  }    


  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  Frechet_precomp< Traits, BidirectionalIter >::
  ~Frechet_precomp()
  //-------------------------------------------------- 
  {
    // EMPTY
  }    




  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  void
  Frechet_precomp< Traits, BidirectionalIter >::deduce
  ( const number_t& , const number_t& )
  //-------------------------------------------------- 
  {
    // EMPTY
  }    




} // namespace ns_parametric_search

