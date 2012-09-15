#ifndef FILTER_H
#define FILTER_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {


// A filter iterator.
//
// The point of this file. Given a pair of iterators of type IterT and a boolean function,
// it can create iterators (a nested subtype) exporting all constant functions of the
// original iterators except []. However, these iterators will skip over those elements
// for which the given function returns false.
//
// Create using Filter(), below.
//

// Usage example:
//
// std::vector<int> v{0, 1, 2, 3, 4, 5, 6};
// auto vf = FIter::Filter(mod2)(v.begin(), v.end());
// for(auto x : vf)
//   std::cout << x << ",";
// 
// This will print '0,2,4,6,', assuming 'mod2' is defined appropriately. (Say, as
// 'bool mod2(int x){return x%2==0;}'.

template<typename IterT>
class FilteredObject {

  typedef typename IterT::value_type value_type;
  // Note: The following is necessary because Filters never reverse iteration.
  typedef typename least_iterator_type<typename IterT::iterator_category, std::forward_iterator_tag>::type least_common_subtype;

 protected:
  const IterT m_begin;
  const IterT m_end;
 
  std::function<bool(value_type)> filter;


 public:
  struct const_iterator : public std::iterator<least_common_subtype, value_type>,
  public Iterator_base<least_common_subtype, const_iterator, value_type>
  {
    // Normally we don't store end, but filter uses them for skipping safely.
    IterT m_cur;
    IterT m_end;
    std::function<bool(value_type)> filter;

    // Get the iterator at the base of a chain of FIters. For example, if you apply a
    // filter to a vector, and then a map to the filter, calling this on the map will
    // return the vector iterator at the current location.
    // See FIter.h for implementation.
    auto get_base() -> decltype(_get_base<IterT>(m_cur, 0)) {
      return _get_base<IterT>(m_cur, 0);
    }
   
    value_type access() const { return *m_cur; }

    void first() { // find the first element passing filter().
      for (; m_cur != m_end; ++m_cur)
        if (filter(*m_cur)) break;
    }

    void advance() { // find the next element passing filter().
      if (m_cur == m_end) return; 
      for (++m_cur; m_cur != m_end; ++m_cur)
        if (filter(*m_cur)) break;
    }


   

    const_iterator(const IterT & _cur, const IterT & _end, std::function<bool(value_type)> _filter) : m_cur(_cur), m_end(_end), filter(_filter)
    { first(); } 

    const_iterator(const const_iterator& r) :  m_cur(r.m_cur),  m_end(r.m_end), filter(r.filter)
    { first(); }
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur = r.m_cur; m_end = r.m_end; filter = r.filter; first(); return *this; }
  };
  

  FilteredObject(IterT _begin, IterT _end, std::function<bool(value_type)> _filter) : m_begin(_begin), m_end(_end), filter(_filter)
  {}
   
  const_iterator begin() const {
    return const_iterator(m_begin, m_end, filter);
  }
    
  const_iterator end() const {
   return const_iterator(m_end, m_end, filter);
  }
};







// Stores a function. When called on a pair of iterators, returns a FilteredObject which
// iterates between them using the passed function as a filter.
// 'func' is 'std::function<bool, ValueT>', in this case.
// Its purposes are to allow currying and implicit template instantiation.
template <typename func>
class FilterOn {
  public:
  func f;
  
  FilterOn(func _f) : f(_f) {}
  
  template <typename IterT>
  FilteredObject<IterT> operator() (IterT start, IterT end) {
    return FilteredObject<IterT>(start, end, f);
  }
};







// Filter takes a boolean function and returns a FilterOn<> storing that function.
// Only necessary to allow implicit template instantiation and lambdas.
// Two versions: this for callable objects, the next for function pointers.
template<typename F>
auto Filter(F f) -> FilterOn<std::function<bool(typename function_traits<decltype(&F::operator())>::arg_type)> >
{
  return FilterOn<std::function<bool(typename function_traits<decltype(&F::operator())>::arg_type)>>(f);
}

template<typename A>
FilterOn<std::function<bool(A)>> Filter(bool(&f)(A)) {
  return FilterOn<std::function<bool(A)>>(f);
}


}

#endif
