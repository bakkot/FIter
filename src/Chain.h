#ifndef CHAIN_H
#define CHAIN_H

#include <functional>
#include <iterator>
#include "FIter.h"

namespace FIter {



// A 'chaining' iterator.
//
// The point of this file. Given two iterators, it returns an iterator over the two taken
// sequentially (like concatenation).
//
// Create using Chain(), below.
//
// Note that the type of the second iterator must be able to be coerced to the type of
// the first. If this condition is not met, the code should fail to compile with an
// 'invalid conversion' error: however, this won't happen unless your code actually uses
// the iterator, because otherwise the function causing this error won't be generated.
//

// Usage example:
//
// std::vector<int> v1{0, 1, 2};
// std::vector<int> v2{3, 4, 5, 6};
// auto vc = FIter::Chain(v1.begin(), v1.end())(v2.begin(), v2.end());
// for(auto x : vc)
//   std::cout << x << ',';
// 
// This will print '0,1,2,3,4,5,6,'

template<typename IterT_1, typename IterT_2>
class ChainObject {


	// the value types of the sub-iterators.
	
	typedef typename IterT_1::value_type value_type_1;
	typedef typename IterT_2::value_type value_type_2;

	// the type of this iterator must be unchanging, so value_type_2 will need to be able to
	// be coerced to match value_type_1
  typedef value_type_1 value_type;
  
	// Only support input / forward iterators.
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
    IterT_1 m_end_1;
    IterT_2 m_cur_2;

   
		const_iterator& operator++() { 
			if(m_cur_1 != m_end_1) {
				++m_cur_1;
			}
			else {
				++m_cur_2;
			}
			return *this;
		}
		const_iterator operator++(int) {
			auto tmp = *this;
			if(m_cur_1 != m_end_1) {
				++m_cur_1;
			}
			else {
				++m_cur_2;
			}
			return tmp;
		}  
		value_type operator*() const {
			if(m_cur_1 != m_end_1)
				return *m_cur_1;
			else
				return *m_cur_2;
		}  
		value_type* operator->() const { 
			if(m_cur_1 != m_end_1)
				return &(*m_cur_1);
			else
				return &(*m_cur_2);
		}
		
		// end once BOTH end.
		bool operator==(const const_iterator& r) const { return (m_cur_1 == r.m_cur_1 && m_cur_2 == r.m_cur_2); }
		bool operator!=(const const_iterator& r) const { return !(operator==(r)); }


   

    const_iterator(const IterT_1& _cur_1, const IterT_1& _end_1, const IterT_2& _cur_2) : m_cur_1(_cur_1), m_end_1(_end_1), m_cur_2(_cur_2)
    {} 

    const_iterator(const const_iterator& r) : m_cur_1(r.m_cur_1), m_end_1(r.m_end_1), m_cur_2(r.m_cur_2)
    {}
    
    const_iterator& operator=(const const_iterator& r)
    { m_cur_1 = r.m_cur_1; m_end_1 = r.m_end_1; m_cur_2 = r.m_cur_2; return *this; }
  };
  

  ChainObject(IterT_1 _begin_1, IterT_1 _end_1, IterT_2 _begin_2, IterT_2 _end_2) :
  	m_begin_1(_begin_1), m_end_1(_end_1), m_begin_2(_begin_2), m_end_2(_end_2)
  {}
   
  const_iterator begin() const {
    return const_iterator(m_begin_1, m_end_1, m_begin_2);
  }
    
  const_iterator end() const {
   return const_iterator(m_end_1, m_end_1, m_end_2);
  }
};







// Stores a pair of iterators. When called on a pair of iterators, returns a ChainObject
// iterating over the two in succession.
// Its purposes are to allow currying and implicit template instantiation.
template <typename IterT_1>
struct ChainWith {
	IterT_1 begin_1;
	IterT_1 end_1;
  
  ChainWith(IterT_1 _begin_1, IterT_1 _end_1) : begin_1(_begin_1), end_1(_end_1) {}
  
  template <typename IterT_2>
  ChainObject<IterT_1, IterT_2> operator() (IterT_2 begin_2, IterT_2 end_2) {
    return ChainObject<IterT_1, IterT_2>(begin_1, end_1, begin_2, end_2);
  }
};







// Chain takes a pair of iterators and returns a ChainWith<> storing those itertors.
// Only necessary to allow implicit template instantiation.

template<typename IterT_1>
ChainWith<IterT_1> Chain(IterT_1 begin, IterT_1 end) {
  return ChainWith<IterT_1>(begin, end);
}





}

#endif
