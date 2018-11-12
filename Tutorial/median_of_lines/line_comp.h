// file line_comp.h

#ifndef LINE_COMP_H
#define LINE_COMP_H

//-------------------------------------------------- 
template< class Traits >
class Line_comp : public Traits::comparison_base_t
//-------------------------------------------------- 
{
public: // type definitions
  typedef typename Traits::process_base_t    process_base_t;
  typedef typename Traits::comparison_base_t comparison_base_t;
  typedef typename Traits::number_t          number_t;
  typedef typename Traits::polynomial_t      polynomial_t;

public: // member functions
  virtual ~Line_comp();

  static comparison_base_t* lazy_create_comparison
  ( process_base_t* parent,
    bool& comp_result,
    const polynomial_t& first,
    const polynomial_t& second );

private: // member functions
  // constructor is private. A Line_comp object can and may only be
  // created by its static member function lazy_create_comparison

  Line_comp( process_base_t* parent,
	     bool& comp_result,
	     const polynomial_t& first,
	     const polynomial_t& second );
  virtual void deduce( const number_t& lower_bound,
		       const number_t& upper_bound );

private: // data members
  bool& comp_result_;
  polynomial_t first_;
  polynomial_t second_;
  number_t root_value_;

private: // static data member
  static number_t static_root_value_;
};

#endif // LINE_COMP_H
