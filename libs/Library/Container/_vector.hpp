#ifndef _VECTOR_HPP
# define _VECTOR_HPP

#include "_itertatorBaseFunc.hpp"
#include "_iterator.hpp"
#include "_alloc.hpp"
#include <memory>
#include <cstddef>
#include "_construct.hpp"
#include <vector>

// #include <allocator.h>
// #include <allocator.h>
namespace ft{

//std::addressof<- can get Object's address even if overloaded operator&()
//copy from __f to __l into ForwardIterator __r
template <class _InputIterator, class _ForwardIterator>
_ForwardIterator
uninitialized_copy(_InputIterator __f, _InputIterator __l, _ForwardIterator __r)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
// #ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __r;
    try
    {
// #endif
        for (; __f != __l; ++__f, (void) ++__r)
            ::new ((void*)std::addressof(*__r)) value_type(*__f);
// #ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __r; ++__s)
            __s->~value_type();
        throw;
    }
// #endif
    return __r;
}

template <class _ForwardIterator, class _Size, class _Tp>
_ForwardIterator
uninitialized_fill_n(_ForwardIterator __f, _Size __n, const _Tp& __x)
{
    typedef typename iterator_traits<_ForwardIterator>::value_type value_type;
// #ifndef _LIBCPP_NO_EXCEPTIONS
    _ForwardIterator __s = __f;
    try
    {
// #endif
        // (void ) --__n Sementic:
        // The (void) cast before --__n is used to explicitly ignore the result of the decrement operation. 
        // This is done for two primary reasons:
        // 1. To indicate that the result of the operation is intentionally not being used, 
        //    making the code's intent clearer to future developers who might work on this code.
        // 2. To potentially suppress compiler warnings that might arise when the result 
        //    of an expression is not used, hence promoting cleaner, warning-free code compilation.

        for (; __n > 0; ++__f, (void) --__n)
            ::new ((void*)std::addressof(*__f)) value_type(__x);
// #ifndef _LIBCPP_NO_EXCEPTIONS
    }
    catch (...)
    {
        for (; __s != __f; ++__s)
            __s->~value_type();
        throw;
    }
// #endif
    return __f;
}
//  Understanding the _IsStatic Template Parameter in STL Vector Implementation
// Introduction
// The _IsStatic template parameter in the internal implementation of the STL vector plays a pivotal role in determining the behavior and properties of the allocator associated with the vector. It essentially marks whether the allocator is stateless (does not maintain internal state information) or stateful (maintains internal state information). Here, we delve into why it is essential and how it influences the vector's performance and memory efficiency.

// Stateless Allocators
// When _IsStatic is set to true, it signifies that the allocator is stateless, indicating that it does not store any internal state information. Stateless allocators generally perform memory allocation and deallocation operations through function calls, without maintaining any instance-specific data.

// A typical example of a stateless allocator is std::allocator, where all instances of std::allocator<T> behave identically. Since these allocators do not have a distinct behavior between instances, it is unnecessary for each vector object to maintain its allocator instance.

// By not storing an allocator instance in each vector object, we can save memory space, making each vector object smaller. This not only helps in memory conservation but also optimizes CPU cache efficiency, enhancing overall performance.

// Stateful Allocators
// In contrast, stateful allocators have internal states that dictate how memory allocations and deallocations are performed. Different instances of a stateful allocator might have different behaviors, making it necessary for each vector object to maintain its instance of the allocator.

// For example, a memory pool allocator, a type of stateful allocator, can allocate and deallocate objects from a pre-allocated pool of memory blocks, reducing the overhead of memory allocations.

// Implementation Details
// In the context of the _Vector_alloc_base template, when _IsStatic is set to true, it does not store the _M_data_allocator member variable, which would otherwise hold an instance of the allocator. Instead, it calls static member functions of the allocator type to allocate and deallocate memory. This approach avoids the necessity to store an allocator instance per vector object, saving memory and reducing CPU overhead.

// When dealing with stateful allocators, the _IsStatic parameter would be set to false, indicating that each vector object needs to maintain its allocator instance to cater to the unique behavior of the allocator.

// Conclusion
// The _IsStatic template parameter serves a critical role in optimizing the STL vector's performance and memory usage. By distinguishing between stateless and stateful allocators, it allows for more efficient memory utilization and improved CPU cache efficiency, particularly when working with stateless allocators. This parameter ensures that the vector implementation can work harmoniously with both stateless and stateful allocators, offering flexibility and optimization in various use-cases.   



// The vector base class serves two purposes.  First, its constructor
// and destructor allocate (but don't initialize) storage.  This makes
// exception safety easier.  Second, the base class encapsulates all of
// the differences between SGI-style allocators and standard-conforming
// allocators.


//Base class for ordinary allocators.
template <class _Tp, class _Allocator, bool _IsStatic>
class _Vector_alloc_base{
public:
    typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type allocator_type;
    allocator_type get_allocator() const { return _M_data_allocator; }

    _Vector_alloc_base(const allocator_type& __a)
    : _M_data_allocator(__a), _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
protected:
    allocator_type _M_data_allocator;
    _Tp* _M_start;
    //pointer behind the last element
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    
    _Tp* _M_allocate(size_t __n){
        return _M_data_allocator.allocate(__n);
    }
    void _M_deallocate(_Tp* __p, size_t __n){
        if(__p)
            _M_data_allocator.deallocate(__p, __n);
    }
};
//specialization for stateless allocators.
// This specialization is optimized for the case of stateless allocators
// have not to store an allocator instance in each vector object, we can save memory space, making each vector object smaller. This not only helps in memory conservation but also optimizes CPU cache efficiency, enhancing overall performance.
template <class _Tp, class _Allocator>
class _Vector_alloc_base<_Tp, _Allocator, true>{
public:
    typedef typename _Alloc_traits<_Tp, _Allocator>::allocator_type allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }

    _Vector_alloc_base(const allocator_type&) : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    
    typedef typename _Alloc_traits<_Tp, _Allocator>::_Alloc_type _Alloc_type;
    _Tp* _M_allocate(size_t __n){
        return _Alloc_type::allocate(__n);
    }
    void _M_deallocate(_Tp* __p, size_t __n){
        _Alloc_type::deallocate(__p, __n);
    }
};

template <typename _Tp, typename _Alloc>
struct _Vectorbase
    : public _Vector_alloc_base<_Tp, _Alloc,
                            _Alloc_traits<_Tp, _Alloc>::_S_instanceless>
{
    typedef _Vector_alloc_base<_Tp, _Alloc,
                            _Alloc_traits<_Tp, _Alloc>::_S_instanceless> 
            _Base;
    typedef typename _Base::allocator_type allocator_type;

    _Vectorbase(const allocator_type& __a) : _Base(__a) {}
    _Vectorbase(size_t __n, const allocator_type& __a) : _Base(__a){
        _M_start = _M_allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + __n;
    }
    ~_Vectorbase(){
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    }

};

//protected inheritance for capusulating the vectorBase
template <typename _Tp, typename _Alloc = allocator<_Tp> >
class vector : protected _Vectorbase<_Tp, _Alloc>{
    // concept requirements
    // _ft_class_requires(_Tp, _Assignable);
private:
    typedef _Vectorbase<_Tp, _Alloc> _Base;
    typedef vector<_Tp, _Alloc> vector_type;
public:
    typedef _Tp value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef __normal_iterator<pointer, vector_type> iterator;
    typedef __normal_iterator<const_pointer, vector_type> const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef typename _Base::allocator_type allocator_type;
    allocator_type get_allocator() const { return _Base::get_allocator(); }

    typedef reverse_iterator<const_iterator> const_reverse_iterator;
    typedef reverse_iterator<iterator> reverse_iterator;

protected:
    using _Base::_M_allocate;
    using _Base::_M_deallocate;
    using _Base::_M_start;
    using _Base::_M_finish;
    using _Base::_M_end_of_storage;

protected:
    //method for memory allocation auxiliary
    void _M_insert_aux(iterator __position, const _Tp& __x);
    void _M_insert_aux(iterator __position);

public:
    iterator begin() { return iterator(_M_start); }
    const_iterator begin() const { return const_iterator(_M_start); }
    iterator end() { return iterator(_M_finish); }
    const_iterator end() const { return const_iterator(_M_finish); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

    size_type size() const { return size_type(end() - begin()); }
    size_type max_size() const { return size_type(-1) / sizeof(_Tp); }
    size_type capacity() const { return size_type(const_iterator(_M_end_of_storage) - begin()); }
    bool empty() const { return begin() == end(); }

    reference operator[](size_type __n) { return *(begin() + __n); }
    const_reference operator[](size_type __n) const { return *(begin() + __n); }

    void _M_range_check(size_type __n) const {
        if(__n >= this->size())
            throw std::out_of_range("vector");
    }

    reference at(size_type __n) { _M_range_check(__n); return (*this)[__n]; }
    const_reference at(size_type __n) const { _M_range_check(__n); return (*this)[__n]; }
    explicit vector(const allocator_type& __a = allocator_type()) : _Base(__a) {}

    vector(size_type __n, const _Tp& __value,
            const allocator_type& __a = allocator_type()) : _Base(__n, __a){
        _M_finish = uninitialized_fill_n(_M_start, __n, __value);
    }

    explicit vector(size_type __n)
        : _Base(__n, allocator_type()){
        _M_finish = uninitialized_fill_n(_M_start, __n, _Tp());
        };

    vector(const vector_type& __x) : _Base(__x.size(), __x.get_allocator()){
        _M_finish = uninitialized_copy(__x.begin(), __x.end(), _M_start);
    }

    // check whether it's an integral type.  If so, it's not an iterator.
    // ex) vector<int> v1(123, -1)
    template <typename _InputIterator>
        vector(_InputIterator __first, _InputIterator __last,
                const allocator_type& __a = allocator_type()) 
        : _Base(__a){

        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_initialize_aux(__first, __last, _Integral());
        }

    template <typename _Integer>
        void _M_initialize_aux(_Integer __n, _Integer __value, __true_type){
            _M_start = _M_allocate(__n);
            _M_end_of_storage = _M_start + __n;
            _M_finish = uninitialized_fill_n(_M_start, __n, __value);
        }

    template <typename _InputIterator>
        void _M_initialize_aux(_InputIterator __first, _InputIterator __last, __false_type){
            typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
            _M_range_initialize(__first, __last, _IterCategory());
        }
    ~vector() { destroy(_M_start, _M_finish); }

    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);
    void reserve(size_type __n){
        if(capacity() < __n){
            const size_type __old_size = size();
            pointer __tmp = _M_allocate_and_copy(__n, _M_start, _M_finish);
            _Destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __tmp;
            _M_finish = __tmp + __old_size;
            _M_end_of_storage = _M_start + __n;
        }
    }

  // assign(), a generalized assignment member function.  Two
  // versions: one that takes a count, and one that takes a range.
  // The range version is a member template, so we dispatch on whether
  // or not the type is an integer.
  void assign(size_type __n, const _Tp& __val) { _M_fill_assign(__n, __val); }
  void _M_fill_assign(size_type __n, const _Tp& __val);

  template <typename _InputIterator>
    void assign(_InputIterator __first, _InputIterator __last) {
      typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
      _M_assign_dispatch(__first, __last, _Integral());
    }

    template <typename _Integer>
    void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type){
        _M_fill_assign((size_type) __n, (_Tp) __val);
    }

    template <typename _InputIterator>
    void _M_assign_dispatch(_InputIterator __first, _InputIterator __last, __false_type){
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_assign_aux(__first, __last, _IterCategory());
    }

    template <typename _InputIterator>
    void _M_assign_aux(_InputIterator __first, _InputIterator __last, input_iterator_tag);
    template <typename _ForwardIterator>
    void _M_assign_aux(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag);

    reference front() { return *begin(); }
    const_reference front() const { return *begin(); }
    reference back() { return *(end() - 1); }
    const_reference back() const { return *(end() - 1); }

    void push_back(const _Tp& __x){
        if(_M_finish != _M_end_of_storage){
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_aux(end(), __x);
    }

    void push_back(){
        if(_M_finish != _M_end_of_storage){
            _Construct(_M_finish);
            ++_M_finish;
        }
        else
            _M_insert_aux(end());
    }

    void swap(vector<_Tp, _Alloc>& __x){
        std::swap(_M_start, __x._M_start);
        std::swap(_M_finish, __x._M_finish);
        std::swap(_M_end_of_storage, __x._M_end_of_storage);
    }

    iterator
    insert(iterator __position, const _Tp& __x){
        size_type __n = __position - begin();
        if(_M_finish != _M_end_of_storage && __position == end()){
            _Construct(_M_finish, __x);
            ++_M_finish;
        }
        else
            _M_insert_aux(__position, __x);
        return begin() + __n;
    }

    iterator
    insert(iterator __position){
        size_type __n = __position - begin();
        if(_M_finish != _M_end_of_storage && __position == end()){
            _Construct(_M_finish);
            ++_M_finish;
        }
        else
            _M_insert_aux(__position);
        return begin() + __n;
    }

    // Check whether it's an integral type.  If so, it's not an iterator.
    template <typename _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last){
        typedef typename _Is_integer<_InputIterator>::_Integral _Integral;
        _M_insert_dispatch(__pos, __first, __last, _Integral());
    }
    
    template <typename _Integer>
    void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val, __true_type){
        _M_fill_insert(__pos, (size_type) __n, (_Tp) __val);
    }

    template <typename _InputIterator>
    void _M_insert_dispatch(iterator __pos, _InputIterator __first, _InputIterator __last, __false_type){
        typedef typename iterator_traits<_InputIterator>::iterator_category _IterCategory;
        _M_range_insert(__pos, __first, __last, _IterCategory());
    }

    void insert(iterator __pos, size_type __n, const _Tp& __x){
        _M_fill_insert(__pos, __n, __x);
    }

    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);

    void pop_back(){
        --_M_finish;
        _Destroy(_M_finish);
    }

    iterator erase(iterator __position){
        if(__position + 1 != end())
            std::copy(__position + 1, end(), __position);
        --_M_finish;
        _Destroy(_M_finish);
        return __position;
    }
    iterator erase(iterator __first, iterator __last){
        iterator __i = std::copy(__last, end(), __first);
        _Destroy(__i, _M_finish);
        _M_finish = _M_finish - (__last - __first);
        return __first;
    }

    Void resize(size_type __new_size, const _Tp& __x){
        if(__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(size_type __new_size) { resize(__new_size, _Tp()); }
    void clear() { erase(begin(), end()); }
protected:

    template <typename _ForwardIterator>
    pointer _M_allocate_and_copy(size_type __n, _ForwardIterator __first, _ForwardIterator __last){
        pointer __result = _M_allocate(__n);
        try{
            uninitialized_copy(__first, __last, __result);
            return __result;
        }
        catch(...){
            _M_deallocate(__result, __n);
            throw;
        }
    }

    template <typename _InputIterator>
    void _M_range_initialize(_InputIterator __first, _InputIterator __last, input_iterator_tag){
        for(; __first != __last; ++__first)
            push_back(*__first);
    }

    // This function is only called by the constructor. 
    template <typename _ForwardIterator>
    void _M_range_initialize(_ForwardIterator __first, _ForwardIterator __last, forward_iterator_tag){
        const size_type __n = std::distance(__first, __last);
        _M_start = _M_allocate(__n);
        _M_end_of_storage = _M_start + __n;
        _M_finish = uninitialized_copy(__first, __last, _M_start);
    }

    template <typename _InputIterator>
    void _M_range_insert(iterator __pos,
                        _InputIterator __first, _InputIterator __last,
                        input_iterator_tag);

    template <typename _ForwardIterator>
    void _M_range_insert(iterator __pos,
                        _ForwardIterator __first, _ForwardIterator __last,
                        forward_iterator_tag);
};

template <typename _Tp, typename _Alloc>
inline bool operator==(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y){
    return __x.size() == __y.size() && std::equal(__x.begin(), __x.end(), __y.begin());
}

// Lexicographical ordering-> compare each element of two containers like dictionary
template <typename _Tp, typename _Alloc>
inline bool operator<(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y){
    return std::lexicographical_compare(__x.begin(), __x.end(), __y.begin(), __y.end());
}

template <typename _Tp, typename _Alloc>
inline void swap(vector<_Tp, _Alloc>& __x, vector<_Tp, _Alloc>& __y){
    __x.swap(__y);  
}

template <typename _Tp, typename _Alloc>
inline bool
operator!=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
  return !(__x == __y);
}

template <typename _Tp, typename _Alloc>
inline bool
operator>(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
  return __y < __x;
}

template <typename _Tp, typename _Alloc>
inline bool
operator<=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
  return !(__y < __x);
}

template <class _Tp, class _Alloc>
inline bool
operator>=(const vector<_Tp, _Alloc>& __x, const vector<_Tp, _Alloc>& __y) {
  return !(__x < __y);
}

template <typename _Tp, typename _Alloc>
vector<_Tp, _Alloc>&
vector<_Tp, _Alloc>::operator=(const vector<_Tp, _Alloc>& __x)
{
    if (&__x != this) {
        const size_type __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, __x.begin(), __x.end());
            _Destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen) {
            pointer __i = std::copy(__x.begin(), __x.end(), begin());
            _Destroy(__i, _M_finish);
        }
        else {
            //seperate copy and destroy to avoid self-assignment
            std::copy(__x.begin(), __x.begin() + size(), _M_start);
            uninitialized_copy(__x.begin() + size(), __x.end(), _M_finish);
        }
    }
    return *this;
}

template <typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const value_type& __val) 
{
    // if __n > capacity()-> copy and swap
    // if __n <= capacity() && __n > size() -> fill and uninitialized_fill
    // if __n <= size() -> fill and erase
    if (__n > capacity()) {
        vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
        __tmp.swap(*this);
    }
    else if (__n > size())    {
        std::fill(begin(), end(), __val);
        _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val);
    }
    else
        erase(std::fill_n(begin(), __n, __val), end());
}

template <typename _Tp, typename _Alloc> template <typename _InputIterator>
void vector<_Tp, _Alloc>::_M_assign_aux(_InputIterator __first, _InputIterator __last, input_iterator_tag)
{
    iterator __cur(begin());
    for(; __first != __last && __cur != end(); ++__cur, ++__first)
        *__cur = *__first;
    if(__first == __last)
        erase(__cur, end());
    else
        insert(end(), __first, __last);
}

template <typename _Tp, typename _Alloc> template <typename _ForwardIter>
void vector<_Tp, _Alloc>::_M_assign_aux(_ForwardIter __first, _ForwardIter __last,
                                   forward_iterator_tag){
    size_type __len = distance(__first, __last);

    if (__len > capacity()) {
        pointer __tmp(_M_allocate_and_copy(__len, __first, __last));
        _Destroy(_M_start, _M_finish);
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __tmp;
        _M_end_of_storage = _M_start + __len;
    }
    else if (size() >= __len) {
        iterator __new_finish(std::copy(__first, __last, begin()));
        _Destroy(__new_finish, _M_finish);
    }
    else {
        _ForwardIter __mid = __first;
        advance(__mid, size());
        std::copy(__first, __mid, _M_start);
        _M_finish = uninitialized_copy(__mid, __last, _M_finish);
    }
}

template <typename _Tp, typename _Alloc>
void
vector<_Tp, _Alloc>::_M_insert_aux(iterator __position, const _Tp& __x){
    if (_M_finish != _M_end_of_storage) {
        // M_finish-> does not intialize -> so must use construct
        _Construct(_M_finish, *(_M_finish - 1));
        ++_M_finish;
        _Tp __x_copy = __x;
        std::copy_backward(__position, _M_finish - 2, _M_finish - 1);
        *__position = __x_copy;
    }
    else {
        const size_type __old_size = size();
        const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
        pointer __new_start = _M_allocate(__len);
        pointer __new_finish = __new_start;
        try {
            __new_finish = uninitialized_copy(_M_start, __position, __new_start);
            _Construct(__new_finish, __x);
            ++__new_finish;
            __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
        }
        catch(...) {
            _Destroy(__new_start, __new_finish);
            _M_deallocate(__new_start, __len);
            throw;
        }
        _Destroy(begin(), end());
        _M_deallocate(_M_start, _M_end_of_storage - _M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __len;
    }
}

template <class _Tp, class _Alloc>
void 
vector<_Tp, _Alloc>::_M_insert_aux(iterator __position)
{
  if (_M_finish != _M_end_of_storage) {
    _Construct(_M_finish, *(_M_finish - 1));
    ++_M_finish;
    std::copy_backward(__position, iterator(_M_finish - 2), 
		  iterator(_M_finish - 1));
    *__position = _Tp();
  }
  else {
    const size_type __old_size = size();
    const size_type __len = __old_size != 0 ? 2 * __old_size : 1;
    pointer __new_start = _M_allocate(__len);
    pointer __new_finish = __new_start;
    try {
      __new_finish = uninitialized_copy(iterator(_M_start), __position, 
					__new_start);
      _Construct(__new_finish);
      ++__new_finish;
      __new_finish = uninitialized_copy(__position, iterator(_M_finish), 
					__new_finish);
    }
    catch(...)
      {
	_Destroy(__new_start,__new_finish); 
	_M_deallocate(__new_start,__len);
	__throw_exception_again;
      }
    _Destroy(begin(), end());
    _M_deallocate(_M_start, _M_end_of_storage - _M_start);
    _M_start = __new_start;
    _M_finish = __new_finish;
    _M_end_of_storage = __new_start + __len;
  }
}

template <typename _Tp, typename _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, 
                                         const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(_M_end_of_storage - _M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = _M_finish - __position;
            pointer __old_finish = _M_finish;
            if (__elems_after > __n) {
                uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
                _M_finish += __n;
                std::copy_backward(__position, __old_finish - __n, __old_finish);
                std::fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(_M_finish, __n - __elems_after, __x_copy);
                _M_finish += __n - __elems_after;
                uninitialized_copy(__position, __old_finish, _M_finish);
                _M_finish += __elems_after;
                std::fill(__position, __old_finish, __x_copy);
            }
        }
        else {
            const size_type __old_size = size();
            const size_type __len = __old_size + std::max(__old_size, __n);
            pointer __new_start = _M_allocate(__len);
            pointer __new_finish = __new_start;
            try {
                __new_finish = uninitialized_copy(_M_start, __position, __new_start);
                __new_finish = uninitialized_fill_n(__new_finish, __n, __x);
                __new_finish = uninitialized_copy(__position, _M_finish, __new_finish);
            }
            catch(...) {
                _Destroy(__new_start, __new_finish);
                _M_deallocate(__new_start, __len);
                throw;
            }
            _Destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __new_start;
            _M_finish = __new_finish;
            _M_end_of_storage = __new_start + __len;
        }
    }
}                                         
template <typename _Tp, typename _Alloc> template <typename _InputIterator>
void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos,
                                          _InputIterator __first, _InputIterator __last,
                                          input_iterator_tag)
{
    for(; __first != __last; ++__first){
        __pos = insert(__pos, *__first);
        ++__pos;
    }
}
template <typename _Tp, typename _Alloc> template <typename _ForwardIterator>
void vector<_Tp, _Alloc>::_M_range_insert(iterator __pos,
                                          _ForwardIterator __first, _ForwardIterator __last,
                                          forward_iterator_tag)
{
    if(__first != __last){
        size_type __n = distance(__first, __last, __n);
        if(size_type(_M_end_of_storage - _M_finish) >= __n){
            const size_type __elems_after = _M_finish - __pos;
            pointer __old_finish = _M_finish;
            if(__elems_after > __n){
                uninitialized_copy(_M_finish - __n, _M_finish, _M_finish);
                _M_finish += __n;
                std::copy_backward(__pos, __old_finish - __n, __old_finish);
                std::copy(__first, __last, __pos);
            }
            else{
                _ForwardIterator __mid = __first;
                advance(__mid, __elems_after);
                uninitialized_copy(__mid, __last, _M_finish);
                _M_finish += __n - __elems_after;
                uninitialized_copy(__pos, __old_finish, _M_finish);
                _M_finish += __elems_after;
                std::copy(__first, __mid, __pos);
            }
        }
        else{
            const size_type __old_size = size();
            const size_type __len = __old_size + std::max(__old_size, __n);
            pointer __new_start = _M_allocate(__len);
            pointer __new_finish = __new_start;
            try{
                __new_finish = uninitialized_copy(_M_start, __pos, __new_start);
                __new_finish = uninitialized_copy(__first, __last, __new_finish);
                __new_finish = uninitialized_copy(__pos, _M_finish, __new_finish);
            }
            catch(...){
                _Destroy(__new_start, __new_finish);
                _M_deallocate(__new_start, __len);
                throw;
            }
            _Destroy(_M_start, _M_finish);
            _M_deallocate(_M_start, _M_end_of_storage - _M_start);
            _M_start = __new_start;
            _M_finish = __new_finish;
            _M_end_of_storage = __new_start + __len;
        }
    }
}
};
#endif