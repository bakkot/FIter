#ifndef DROPWHILE_H
#define DROPWHILE_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {


// A 'drop-while' iterator.
//
// The point of this file. Given a pair of iterators of type IterT and a boolean function,
// it can create forward iterators (a nested subtype) from the passed pair. However, these
// created iterators begin starting at the first element for which the function returns
// false: elements are "dropped while" the condition is true.
//
// If every element passes the predicate function, returns an empty iterator (that is,
// begin()==end()).
//
// Create using DropWhile(), below.
//

// Usage example:
//
// std::vector<int> v{0, 1, 2, 3, 4, 5, 6};
// auto vdw = FIter::DropWhile([](int x){return x<3;})(v.begin(), v.end());
// for(auto x : vtw)
//   std::cout << x << ",";
// 
// This will print '3,4,5,6,'.

template<typename IterT>
class DropWhileObject {

  typedef typename IterT::value_type value_type;
  // Note: The following is necessary because DropWhileObjects never support reverse iteration.
  typedef typename least_iterator_type<typename IterT::iterator_category, std::forward_iterator_tag>::type least_common_subtype;

 protected:
  const IterT m_begin;
  const IterT m_end;
 
  std::function<bool(value_type)> whilef;


 public:
 	// Actually one of the simplest types; doesn't even need its own type, really, because
 	// we could just use the parent. Included for completeness, though.
  struct const_iterator : public std::iterator<least_common_subtype, value_type>,
  public Iterator_base<least_common_subtype, const_iterator, value_type>
  {
    IterT m_cur;

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

    const_iterator(const IterT & _cur) : m_cur(_cur)
    {} 

    const_iterator(const const_iterator& r) :  m_cur(r.m_cur)
    {}
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur = r.m_cur; return *this; }
  };
  

  DropWhileObject(IterT _begin, IterT _end, std::function<bool(value_type)> _whilef) : m_begin(_begin), m_end(_end), whilef(_whilef)
  {}
   
  const_iterator begin() const {
  	auto t_advanced = m_begin;
  	while(whilef(*t_advanced) && t_advanced != m_end) {
  		++t_advanced;
  	}
    return const_iterator(t_advanced);
  }
    
  const_iterator end() const {
   return const_iterator(m_end);
  }
};







// Stores a boolean function f. When called on a pair of iterators, returns a
// DropWhileObject which iterates between them starting when f(item) is first false.
// 'func' is 'std::function<bool, ValueT>', in this case.
// Its purposes are to allow currying and implicit template instantiation.
template <typename func>
class DropWhileOn {
  public:
  func f;
  
  DropWhileOn(func _f) : f(_f) {}
  
  template <typename IterT>
  DropWhileObject<IterT> operator() (IterT start, IterT end) {
    return DropWhileObject<IterT>(start, end, f);
  }
};





// DropWhile takes a boolean function and returns a DropWhileOn<> storing that function.
// Only necessary to allow implicit template instantiation and lambdas.
// Two versions: this for callable objects, the next for function pointers.
template<typename F>
auto DropWhile(F f) -> DropWhileOn<std::function<bool(typename function_traits<decltype(&F::operator())>::arg_type)> >
{
  return DropWhileOn<std::function<bool(typename function_traits<decltype(&F::operator())>::arg_type)>>(f);
}

template<typename A>
DropWhileOn<std::function<bool(A)>> DropWhile(bool(&f)(A)) {
  return DropWhileOn<std::function<bool(A)>>(f);
}




}

#endif
