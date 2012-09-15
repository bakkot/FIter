#ifndef PROGRESSION_H
#define PROGRESSION_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {

// An infinite arithmetic progression iterator.
//
// WARNING: INFINITE (comparing to .end() always returns false). This can VERY EASILY put
// infinite loops in your code; do not iterate over this using ranges or .end(). You
// should ALWAYS use Take or TakeWhile with this, if not outputting a predetermined number
// of values.
//
// Given a pair of objects A, B of the same type, outputs A, A+B, (A+B)+B, etc.
//
// Create using Progression(), below.

// Usage example:
//
//  auto tens = FIter::Progression(3, 10); // Count from 3 by 10s.
//  auto tensit = tens.begin();
//  for(int i=0; i<5; ++i) {
//    std::cout << *tensit << ",";
//    ++tensit;
//  }
//
//  This will print '3,13,23,33,43,'.

template<typename ValueT>
class ProgressionObject {
  ValueT start;
  ValueT step;

 public:
  class const_iterator : public std::iterator<std::forward_iterator_tag, ValueT> {
    typedef ValueT value_type;
    value_type current;
    value_type step;
    bool is_end;
    
   public:
    const_iterator& operator++() {
      current += step;
      return *this;
    }
    const_iterator operator++(int) {
      auto tmp = *this;
      current += step;
      return tmp;
    }
    value_type operator*() {
      return current;
    }
    value_type* operator->() {
      return &current;
    }
    bool operator==(const const_iterator& r) {
      return is_end == r.is_end && current == r.current;
    }
    bool operator!=(const const_iterator& r) {
      return !(operator==(r));
    }
    
    const_iterator(value_type _current, value_type _step, bool _is_end) :
     current(_current), step(_step), is_end(_is_end)
    {}
  };
  
  ProgressionObject(ValueT _start, ValueT _step) :
   start(_start), step(_step)
  {}
  
  const_iterator begin() {
    return const_iterator(start, step, false);
  }
  
  const_iterator end() {
    return const_iterator(start, step, true);
  }
};






// Create a ProgressionObject counting longs from 0 by 1: 0,1,2,...
ProgressionObject<long> Progression() {
  return ProgressionObject<long>(0, 1);
}

// Create a ProgressionObject counting from start by 1s: start, (start+1),...
// Requires that start has +(int) defined. 
template<typename value_type>
ProgressionObject<value_type> Progression(value_type start) {
  return ProgressionObject<value_type>(start, 1);
}

// Create a ProgressionObject counting from start by steps: start, (start+step),...
// Requires that start has +(typeof(start)) defined.
template<typename value_type>
ProgressionObject<value_type> Progression(value_type start, value_type step) {
  return ProgressionObject<value_type>(start, step);
}

}

#endif
