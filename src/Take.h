#ifndef TAKE_H
#define TAKE_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {


// A 'take' iterator.
//
// The point of this file. Given a pair of iterators of type IterT and a positive integer,
// it can create forward iterators (a nested subtype) from the passed pair. However, these
// created iterators end after the given number of elements. Particularly useful for
// infinite lists.
//
// Create using Take(), below.
//

// Usage example:
//
// std::vector<int> v{0, 1, 2, 3, 4, 5, 6};
// auto vt = FIter::Take(3)(v.begin(), v.end());
// for(auto x : vt)
//   std::cout << x << ",";
// 
// This will print '0,1,2'.

template<typename IterT>
class TakeObject {

  typedef typename IterT::value_type value_type;
  // Note: The following is necessary because TakeObjects never support reverse iteration.
  typedef typename least_iterator_type<typename IterT::iterator_category, std::forward_iterator_tag>::type least_common_subtype;

 protected:
  const IterT m_begin;
  const IterT m_end;
 
  const long to_take;


 public:
  struct const_iterator : public std::iterator<least_common_subtype, value_type>,
  public Iterator_base<least_common_subtype, const_iterator, value_type>
  {
    IterT m_cur;
    long to_take;
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
			--to_take;
			++m_cur;
    }
    
    // Take handles comparisons differently, to support ending in the right place.
    bool operator==(const const_iterator& r) const {
      if(to_take == 0 && r.is_end) {
        return true;
      }
      else if(is_end && r.is_end) {
        return true;
      }
      else {
        return (is_end == r.is_end) && (m_cur == r.m_cur) && (to_take == r.to_take);
      }
    }

    const_iterator(const IterT & _cur, long _to_take, bool _is_end) : m_cur(_cur), to_take(_to_take), is_end(_is_end)
    {} 

    const_iterator(const const_iterator& r) :  m_cur(r.m_cur),  to_take(r.to_take), is_end(r.is_end)
    {}
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur = r.m_cur; to_take = r.to_take; is_end = r.is_end; return *this; }
  };
  

  TakeObject(IterT _begin, IterT _end, long _to_take) : m_begin(_begin), m_end(_end), to_take(_to_take)
  {}
   
  const_iterator begin() const {
    return const_iterator(m_begin, to_take, false);
  }
    
  const_iterator end() const {
   return const_iterator(m_begin, 0, true);
  }
};







// Stores an integer n. When called on a pair of iterators, returns a TakeObject which
// iterates between the first n of them.
// 'func' is 'std::function<bool, ValueT>', in this case.
// Its purposes are to allow currying and implicit template instantiation.
class Take {
  public:
  long n;
  
  Take(long _n) : n(_n) {}
  
  template <typename IterT>
  TakeObject<IterT> operator() (IterT start, IterT end) {
    return TakeObject<IterT>(start, end, n);
  }
};




}

#endif
