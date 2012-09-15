#ifndef FITER_FITER_H
#define FITER_FITER_H

#include <functional>
#include <iterator>

namespace FIter {



// Used to determine which functions a pair of iterator types have in common.
template <class, class>
struct least_iterator_type;

template <> struct least_iterator_type<std::input_iterator_tag, std::input_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::input_iterator_tag, std::forward_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::input_iterator_tag, std::bidirectional_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::input_iterator_tag, std::random_access_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::forward_iterator_tag, std::input_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::forward_iterator_tag, std::forward_iterator_tag> {typedef std::forward_iterator_tag type;};
template <> struct least_iterator_type<std::forward_iterator_tag, std::bidirectional_iterator_tag> {typedef std::forward_iterator_tag type;};
template <> struct least_iterator_type<std::forward_iterator_tag, std::random_access_iterator_tag> {typedef std::forward_iterator_tag type;};
template <> struct least_iterator_type<std::bidirectional_iterator_tag, std::input_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::bidirectional_iterator_tag, std::forward_iterator_tag> {typedef std::forward_iterator_tag type;};
template <> struct least_iterator_type<std::bidirectional_iterator_tag, std::bidirectional_iterator_tag> {typedef std::bidirectional_iterator_tag type;};
template <> struct least_iterator_type<std::bidirectional_iterator_tag, std::random_access_iterator_tag> {typedef std::bidirectional_iterator_tag type;};
template <> struct least_iterator_type<std::random_access_iterator_tag, std::input_iterator_tag> {typedef std::input_iterator_tag type;};
template <> struct least_iterator_type<std::random_access_iterator_tag, std::forward_iterator_tag> {typedef std::forward_iterator_tag type;};
template <> struct least_iterator_type<std::random_access_iterator_tag, std::bidirectional_iterator_tag> {typedef std::bidirectional_iterator_tag type;};
template <> struct least_iterator_type<std::random_access_iterator_tag, std::random_access_iterator_tag> {typedef std::random_access_iterator_tag type;};







// Base classes from which to inherit most functionality (++, etc). Overload as needed.
// By specifying the type of iterator, only those functions that type supports will be
// defined.
// In order to inherit from this base class, a class must supply, at least, an m_cur 
// member and access() and advance() methods, as well as unadvance() if the class supports
// backwards iteration. If the class supports random access it must support access(n).
template <class Tag, class IterT, class value_type>
class Iterator_base;

template <class IterT, class value_type> // input iterator
class Iterator_base<std::input_iterator_tag, IterT, value_type> {
 private:
	IterT* parent;
 public:
  Iterator_base() { parent = static_cast<IterT*>(this); }
	IterT& operator++() { parent->advance(); return *parent; }
	IterT operator++(int) { auto tmp = *parent; parent->advance(); return tmp;}  
  value_type operator*() const { return parent->access(); }  
  value_type* operator->() const { return &(parent->access()); }
  bool operator==(const IterT& r) const { return (parent->m_cur == r.m_cur); }
  bool operator!=(const IterT& r) const { return !(operator==(r)); }
};

// This provides nothing additional, but inherits from input_iterator
template <class IterT, class value_type> // forward iterator
class Iterator_base<std::forward_iterator_tag, IterT, value_type> : public Iterator_base<std::input_iterator_tag, IterT, value_type> {};


template <class IterT, class value_type> // bidirectional iterator
class Iterator_base<std::bidirectional_iterator_tag, IterT, value_type> : public Iterator_base<std::input_iterator_tag, IterT, value_type> {
 private:
	IterT* parent;
 public:
  Iterator_base() { parent = static_cast<IterT*>(this); }
	IterT& operator--() { --(parent->m_cur); return *parent; }
	IterT operator--(int) { auto tmp = *parent; --(parent->m_cur); return tmp; }  
};

template <class IterT, class value_type> // random access iterator
class Iterator_base<std::random_access_iterator_tag, IterT, value_type> : public Iterator_base<std::bidirectional_iterator_tag, IterT, value_type> {
 private:
	IterT* parent;
 public:
  typedef typename IterT::difference_type difference_type;
  Iterator_base() { parent = static_cast<IterT*>(this); }
	IterT& operator+=(difference_type n) { parent->m_cur += n; return *parent; }
	IterT operator+(difference_type n) { auto tmp = *parent; tmp+=n; return tmp; }
	IterT& operator-=(difference_type n) { parent->m_cur -= n; return *parent; }
	IterT operator-(difference_type n) { auto tmp = *parent; tmp-=n; return tmp; }
	value_type& operator[](int n) { return (parent->access(n)); }
	bool operator<(const IterT& r) { return (parent->m_cur) < r.m_cur; }
	bool operator<=(const IterT& r) { return (parent->m_cur) <= r.m_cur; }
	bool operator>(const IterT& r) { return (parent->m_cur) > r.m_cur; }
	bool operator>=(const IterT& r) { return (parent->m_cur) >= r.m_cur; }
};







// Overloading technique from Xeo:
// http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence/9154394#9154394 
template <class IterT>
auto _get_base(IterT parent, int) -> decltype(parent.get_base()) {
	return parent.get_base();
}

template <class IterT>
IterT _get_base(IterT parent, long) {
	return parent;
}







// Used to provide the argument and return types of a unary function at compile time.
// Technique from Xeo:
// http://stackoverflow.com/a/8712212/1644272
template<class FPtr>
struct function_traits;
template<class R, class C, class A>
struct function_traits<R (C::*)(A)>
{   // non-const specialization
    typedef A arg_type;
    typedef R result_type;
};
template<class R, class C, class A>
struct function_traits<R (C::*)(A) const>
{   // const specialization
    typedef A arg_type;
    typedef R result_type;
};




}
#endif
