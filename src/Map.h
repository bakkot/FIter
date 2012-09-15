#ifndef MAP_H
#define MAP_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {


// A mapping iterator.
//
// The point of this file. Given a pair of iterators of type IterT and a function,
// it can create iterators (a nested subtype) exporting all constant functions of the
// original iterators. However, these iterators, when dereferenced, return the result of
// said function applied to the original value.
//
// Create using Map(), below.
//

// Usage example:
//
// std::vector<int> v{0, 1, 2, 3, 4, 5, 6};
// auto vm = FIter::Map(mod2)(v.begin(), v.end());
// for(auto x : vf)
//   std::cout << x << ',';
// 
// This will print '0,1,0,1,0,1,0,', assuming 'mod2' is defined appropriately. (Say, as
// 'int mod2(int x){return x%2;}'.

template<typename IterT, typename value_type>
class MapObject {

  // Note: input_type is the type stored by the parent iterator
  // whereas value_type is the type stored by _this_ iterator, ie, the result of mapf

  typedef typename IterT::value_type input_type;
  typedef typename IterT::iterator_category iterator_category;

 protected:
  const IterT m_begin;
  const IterT m_end;
 
  std::function<value_type(input_type)> mapf;


 public:
  struct const_iterator : public std::iterator<iterator_category, value_type>,
  public Iterator_base<iterator_category, const_iterator, value_type>
  {
    IterT m_cur;
    std::function<value_type(input_type)> mapf;

    // Get the iterator at the base of a chain of FIters. For example, if you apply a
    // filter to a vector, and then a map to the filter, calling this will return the
    // vector iterator at the current location.
    // See FIter.h for implementation.
    auto get_base() -> decltype(_get_base<IterT>(m_cur, 0)) {
      return _get_base<IterT>(m_cur, 0);
    }
   
    value_type access() const { // The only thing Maps actually do.
      return mapf(*m_cur);
    }

    void advance() { // Nothing special for map.
      ++m_cur;
    }


   

    const_iterator(const IterT & _cur, std::function<value_type(input_type)> _mapf) : m_cur(_cur), mapf(_mapf)
    {} 

    const_iterator(const const_iterator& r) : m_cur(r.m_cur), mapf(r.mapf)
    {}
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur = r.m_cur; mapf = r.mapf; return *this; }
  };
  

  MapObject(IterT _begin, IterT _end, std::function<value_type(input_type)> _mapf) : m_begin(_begin), m_end(_end), mapf(_mapf)
  {}
   
  const_iterator begin() const {
    return const_iterator(m_begin, mapf);
  }
    
  const_iterator end() const {
   return const_iterator(m_end, mapf);
  }
};







// Stores a function. When called on a pair of iterators, returns a MapObject which
// iterates between them and applies the passed function before returning values.
// Its purposes are to allow currying and implicit template instantiation.
template <typename func>
struct MapOn {
  typedef typename function_traits<decltype(&func::operator())>::result_type value_type;
  func f;
  
  MapOn(func _f) : f(_f) {}
  
  template <typename IterT>
  MapObject<IterT, value_type> operator() (IterT start, IterT end) {
    return MapObject<IterT, value_type>(start, end, f);
  }
};







// Map takes a function and returns a MapOn<> storing that function.
// Only necessary to allow implicit template instantiation and lambdas.
// Two versions: this for callable objects, the next for function pointers.
template<typename F>
auto Map(F f) -> MapOn<std::function<typename function_traits<decltype(&F::operator())>::result_type (typename function_traits<decltype(&F::operator())>::arg_type)> >
{
  return MapOn<std::function<typename function_traits<decltype(&F::operator())>::result_type(typename function_traits<decltype(&F::operator())>::arg_type)>>(f);
}

template<typename R, typename A>
MapOn<std::function<R(A)>> Map(R(&f)(A)) {
  return Map<std::function<R(A)>>(f);
}





}

#endif
