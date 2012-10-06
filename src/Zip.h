#ifndef ZIP_H
#define ZIP_H

#include <functional>
#include <iterator>
#include <utility>
#include "FIter.h"

namespace FIter {



// A 'zip' iterator.
//
// The point of this file. Given two iterators, it returns an iterator the elements of
// which are pairs of elements from those iterators.
//
// Create using Zip(), below.
//

// Usage example:
//
// std::vector<int> v1{0, 1, 2, 3, 4, 5, 6};
// std::vector<char> v2{'a', 'b', 'c', 'd', 'e', 'f', 'g'};
// auto vz = FIter::Zip(v1.begin(), v1.end())(v2.begin(), v2.end());
// for(auto x : vz)
//   std::cout << x.first << ',' << x.second << ';';
// 
// This will print '0,a;1,b;2,c;3,d;4,e;5,f;6,g;'

template<typename IterT_1, typename IterT_2>
class ZipObject {


	// value_types 1 and 2 are those of the elements. value_type itself is the type stored
	// by _this_ iterator.
	
	typedef typename IterT_1::value_type value_type_1;
	typedef typename IterT_2::value_type value_type_2;

  typedef std::pair<value_type_1, value_type_2> value_type;

	// TODO: For now we're only supporting forward iterators here, but there's no real reason why that should be so.
  typedef typename least_iterator_type<typename IterT_1::iterator_category, typename IterT_2::iterator_category>::type least_common_subtype_p;
  typedef typename least_iterator_type<least_common_subtype_p, std::forward_iterator_tag>::type least_common_subtype;

 protected:
  const IterT_1 m_begin_1;
  const IterT_1 m_end_1;
  const IterT_2 m_begin_2;
  const IterT_2 m_end_2;


 public:
  struct const_iterator : public std::iterator<least_common_subtype, value_type>,
  public Iterator_base<least_common_subtype, const_iterator, value_type>
  {
    IterT_1 m_cur_1;
    IterT_2 m_cur_2;

   
		const_iterator& operator++() { ++m_cur_1; ++m_cur_2; return *this; }
		const_iterator operator++(int) { auto tmp = *this; ++m_cur_1; ++m_cur_2; return tmp;}  
		value_type operator*() const { return std::make_pair(*m_cur_1, *m_cur_2); }  
		value_type* operator->() const { return &(std::make_pair(*m_cur_1, *m_cur_2)); } // todo pretty sure this dangles.
		
		// end once EITHER ends.
		bool operator==(const const_iterator& r) const { return (m_cur_1 == r.m_cur_1 || m_cur_2 == r.m_cur_2); }
		bool operator!=(const const_iterator& r) const { return !(operator==(r)); }


   

    const_iterator(const IterT_1& _cur_1, const IterT_2& _cur_2) : m_cur_1(_cur_1), m_cur_2(_cur_2)
    {} 

    const_iterator(const const_iterator& r) : m_cur_1(r.m_cur_1), m_cur_2(r.m_cur_2)
    {}
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur_1 = r.m_cur_1; m_cur_2 = r.m_cur_2; return *this; }
  };
  

  ZipObject(IterT_1 _begin_1, IterT_1 _end_1, IterT_2 _begin_2, IterT_2 _end_2) :
  	m_begin_1(_begin_1), m_end_1(_end_1), m_begin_2(_begin_2), m_end_2(_end_2)
  {}
   
  const_iterator begin() const {
    return const_iterator(m_begin_1, m_begin_2);
  }
    
  const_iterator end() const {
   return const_iterator(m_end_1, m_end_2);
  }
};







// Stores a pair of iterators. When called on a pair of iterators, returns a ZipObject
// iterating over pairs, the first element of which is drawn from the first pair here and
// the second of which is drawn from the second pair.
// Its purposes are to allow currying and implicit template instantiation.
template <typename IterT_1>
struct ZipTo {
	IterT_1 begin_1;
	IterT_1 end_1;
  
  ZipTo(IterT_1 _begin_1, IterT_1 _end_1) : begin_1(_begin_1), end_1(_end_1) {}
  
  template <typename IterT_2>
  ZipObject<IterT_1, IterT_2> operator() (IterT_2 begin_2, IterT_2 end_2) {
    return ZipObject<IterT_1, IterT_2>(begin_1, end_1, begin_2, end_2);
  }
};







// Zip takes a pair of iterators and returns a ZipOn<> storing those itertors.
// Only necessary to allow implicit template instantiation.

template<typename IterT_1>
ZipTo<IterT_1> Zip(IterT_1 begin, IterT_1 end) {
  return ZipTo<IterT_1>(begin, end);
}





}

#endif
