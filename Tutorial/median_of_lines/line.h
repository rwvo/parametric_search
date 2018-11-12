// file line.h

#ifndef LINE_H
#define LINE_H

//-------------------------------------------------- 
template< class Traits >
struct Line {
//-------------------------------------------------- 
  typedef typename Traits::number_t number_t;

  // Convenience constructor. Line equation: y = ax + b
  Line( const number_t& a, const number_t& b )
    : a( a ), b( b ) { };

  number_t a, b;
};

#endif // LINE_H
