#ifndef TAKEWHILE_H
#define TAKEWHILE_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {


// A 'take-while' iterator.
//
// The point of this file. Given a pair of iterators of type IterT and a boolean function,
// it can create forward iterators (a nested subtype) from the passed pair. However, these
// created iterators once the function returns false for one of them.
//
// If you ask for more items than you supply, this will only iterate over items supplied.
// (E.g., taking ten items from a two-item list results in two items.)
//
// Create using TakeWhile(), below.
//

// Usage example:
//
// std::vector<int> v{0, 1, 2, 3, 4, 5, 6};
// auto vtw = FIter::TakeWhile([](int x){return x<5;})(v.begin(), v.end());
// for(auto x : vtw)
//   std::cout << x << ",";
// 
// This will print '0,1,2,3,4,'.

template<typename IterT>
class TakeWhileObject {

  typedef typename IterT::value_type value_type;
  // Note: The following is necessary because TakeWhileObjects never support reverse iteration.
  typedef typename least_iterator_type<typename IterT::iterator_category, std::forward_iterator_tag>::type least_common_subtype;

 protected:
  const IterT m_begin;
  const IterT m_end;
 
  std::function<bool(value_type)> whilef;


 public:
  struct const_iterator : public std::iterator<least_common_subtype, value_type>,
  public Iterator_base<least_common_subtype, const_iterator, value_type>
  {
    IterT m_cur;
    std::function<bool(value_type)> whilef;
    bool is_end;

    // Get the iterator at the base of a chain of FIters. For example, if you apply a
    // filter to a vector, and then a map to the filter, calling this on the map will
    // return the vector iterator at the current location.
    // See FIter.h for implementation.
    auto get_base() -> decltype(_get_base<IterT>(m_cur, 0)) {
      return _get_base<IterT>(m_cur, 0);
    }
   
    value_type access() const { return *m_cur; }

    void advance() {
			++m_cur;
    }
    
    // TakeWhile handles comparisons differently, to support ending in the right place.
    bool operator==(const const_iterator& r) const {
      if(r.is_end && !whilef(*m_cur)) {
        return true;
      }
      else if(is_end && r.is_end) { // two end iterators are always identical
        return true;
      }
      else {
        return m_cur == r.m_cur;
      }
    }

    const_iterator(const IterT & _cur, std::function<bool(value_type)> _whilef, bool _is_end) : m_cur(_cur), whilef(_whilef), is_end(_is_end)
    {} 

    const_iterator(const const_iterator& r) :  m_cur(r.m_cur),  whilef(r.whilef), is_end(r.is_end)
    {}
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur = r.m_cur; whilef = r.whilef; is_end = r.is_end; return *this; }
  };
  

  TakeWhileObject(IterT _begin, IterT _end, std::function<bool(value_type)> _whilef) : m_begin(_begin), m_end(_end), whilef(_whilef)
  {}
   
  const_iterator begin() const {
    return const_iterator(m_begin, whilef, false);
  }
    
  const_iterator end() const {
   return const_iterator(m_end, whilef, true);
  }
};







// Stores a boolean function f. When called on a pair of iterators, returns a
// TakeWhileObject which iterates between them until f(item) is false.
// 'func' is 'std::function<bool, ValueT>', in this case.
// Its purposes are to allow currying and implicit template instantiation.
template <typename func>
class TakeWhileOn {
  public:
  func f;
  
  TakeWhileOn(func _f) : f(_f) {}
  
  template <typename IterT>
  TakeWhileObject<IterT> operator() (IterT start, IterT end) {
    return TakeWhileObject<IterT>(start, end, f);
  }
};





// TakeWhile takes a boolean function and returns a TakeWhileOn<> storing that function.
// Only necessary to allow implicit template instantiation and lambdas.
// Two versions: this for callable objects, the next for function pointers.
template<typename F>
auto TakeWhile(F f) -> TakeWhileOn<std::function<bool(typename function_traits<decltype(&F::operator())>::arg_type)> >
{
  return TakeWhileOn<std::function<bool(typename function_traits<decltype(&F::operator())>::arg_type)>>(f);
}

template<typename A>
TakeWhileOn<std::function<bool(A)>> TakeWhile(bool(&f)(A)) {
  return TakeWhileOn<std::function<bool(A)>>(f);
}




}

#endif
