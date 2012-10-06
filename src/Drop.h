#ifndef DROP_H
#define DROP_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {


// A 'drop' iterator.
//
// The point of this file. Given a pair of iterators of type IterT and a positive integer,
// it can create forward iterators (a nested subtype) from the passed pair. However, these
// created iterators begin after the given number of elements.
//
// If you ask for more items than you supply, return an empty iterator (that is,
// begin()==end()).
//
// Create using Drop(), below.
//

// Usage example:
//
// std::vector<int> v{0, 1, 2, 3, 4, 5, 6};
// auto vt = FIter::Drop(3)(v.begin(), v.end());
// for(auto x : vt)
//   std::cout << x << ",";
// 
// This will print '3,4,5,6,'.

template<typename IterT>
class DropObject {

  typedef typename IterT::value_type value_type;
  // Note: The following is necessary because DropObjects never support reverse iteration.
  typedef typename least_iterator_type<typename IterT::iterator_category, std::forward_iterator_tag>::type least_common_subtype;

 protected:
  const IterT m_begin;
  const IterT m_end;
 
  const long to_drop;


 public:
 	// Actually one of the simplest types; doesn't even need its own type, really, because
 	// we could just use the parent with advance(). Included for completeness, though.
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
  

  DropObject(IterT _begin, IterT _end, long _to_drop) : m_begin(_begin), m_end(_end), to_drop(_to_drop)
  {}
   
  const_iterator begin() const {
  	auto t_advanced = m_begin;
  	for(long i=0; i<to_drop && t_advanced != m_end; i++) {
  		++t_advanced;
  	}
    return const_iterator(t_advanced);
  }
    
  const_iterator end() const {
   return const_iterator(m_end);
  }
};







// Stores an integer n. When called on a pair of iterators, returns a DropObject which
// skips the first n of them.
// Its purposes are to allow currying and implicit template instantiation.
class Drop {
  public:
  long n;
  
  Drop(long _n) : n(_n) {}
  
  template <typename IterT>
  DropObject<IterT> operator() (IterT start, IterT end) {
    return DropObject<IterT>(start, end, n);
  }
};




}

#endif
