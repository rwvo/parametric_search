// file frechet_preprocessor.h

#ifndef FRECHET_PREPROCESSOR_H
#define FRECHET_PREPROCESSOR_H

namespace ns_parametric_search {

  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  class Frechet_preprocessor : public Traits::process_base_t
  //--------------------------------------------------
  {
  public:
    typedef typename Traits::process_base_t process_base_t;

  public:
    Frechet_preprocessor( process_base_t* parent,
			  BidirectionalIter first,
			  BidirectionalIter last );
    ~Frechet_preprocessor();

  private:
    void preprocess();

  private:
    BidirectionalIter first_;
    BidirectionalIter last_;
  };



  //--------------------------------------------------
  template< class Traits, class BidirectionalIter >
  class Frechet_precomp : public Traits::comparison_base_t
  //--------------------------------------------------
  {
  public: // type definitions
    typedef typename Traits::process_base_t    process_base_t;
    typedef typename Traits::comparison_base_t comparison_base_t;
    typedef typename Traits::number_t          number_t;

  public: // member functions
    Frechet_precomp( process_base_t* parent,
		     BidirectionalIter first,
		     BidirectionalIter last );
    ~Frechet_precomp();

  private: // member functions
    virtual void deduce( const number_t& ,
			 const number_t& );
  };



} // namespace ns_parametric_search


#endif // FRECHET_PREPROCESSOR_H
