#ifndef ITERATOR_HPP
# define ITERATOR_HPP

#include "_iteratorBaseType.hpp"

namespace ft{
    //Reverse iterator == adaptator of iterator -> Wrapping an iterator
    //Reverse iterator is an iterator adaptor that reverses the direction of a given iterator.
    template<typename _Iterator>
        class reverse_iterator
        : public iterator<typename iterator_traits<_Iterator>::iterator_category,
        typename iterator_traits<_Iterator>::value_type,
        typename iterator_traits<_Iterator>::difference_type,
        typename iterator_traits<_Iterator>::pointer,
        typename iterator_traits<_Iterator>::reference>
    {
        protected:
            _Iterator _M_current;
        public:
            typedef _Iterator iterator_type;
            typedef typename iterator_traits<_Iterator>::difference_type difference_type;
            typedef typename iterator_traits<_Iterator>::reference reference;
            typedef typename iterator_traits<_Iterator>::pointer pointer;
        public:
            reverse_iterator() : {}

/**
 * @brief Constructs a reverse_iterator from an iterator of the same underlying base type.
 *
 * This explicit constructor prevents implicit conversions from the base iterator type to a reverse_iterator. 
 * It facilitates the creation of a reverse_iterator object directly from a base iterator, promoting type safety by requiring explicit constructor invocation for the conversion.
 *
 * @param __x The base iterator from which to create a new reverse_iterator. 
 * The new reverse_iterator is initialized with __x as the underlying base iterator.
 *
 * ### Example
 *
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4};
 *
 * std::vector<int>::iterator it = vec.begin();
 * std::reverse_iterator<std::vector<int>::iterator> rit(it);
 * 
 * // Here, rit is constructed explicitly from the base iterator it, 
 * // thus preventing any accidental or implicit conversions.
 * ```
 */
explicit reverse_iterator(iterator_type __x) : _M_current(__x) {}


            reverse_iterator(const reverse_iterator& __x) : _M_current(__x._M_current) {}

 /**
 * @brief Constructs a reverse_iterator from another reverse_iterator of a potentially different type.
 *
 * This template constructor allows the creation of a reverse_iterator from another reverse_iterator 
 * of a potentially different type, provided there exists a valid conversion relation between the underlying base iterators.
 * It helps to maintain type safety while allowing flexibility in conversions between different kinds of reverse_iterators.
 *
 * @tparam _Iter The type of the other reverse_iterator's underlying base iterator.
 * @param __x The other reverse_iterator from which to create a new reverse_iterator. 
 * The new reverse_iterator is initialized using the base iterator obtained from __x.base().
 *
 * ### Example
 * 
 * ```cpp
 * std::vector<int> vec = {1, 2, 3, 4};
 *
 * std::reverse_iterator<std::vector<int>::iterator> rit1 = vec.rbegin();
 * std::reverse_iterator<std::vector<int>::const_iterator> rit2 = rit1;
 * 
 * // Here, the constructor is invoked with _Iter being std::vector<int>::iterator 
 * // and it allows the conversion to a reverse_iterator with 
 * // _Iter as std::vector<int>::const_iterator due to the valid conversion between the base iterators.
 * ```
 */
            template<typename _Iter>
                reverse_iterator(const reverse_iterator<_Iter>& __x) : _M_current(__x.base()) {}
            iterator_type base() const { return _M_current; }
/**
 * @brief Dereferences the reverse iterator to access the current element it points to in the reversed sequence.
 * 
 * The operator* is designed to reference one element backwards due to the nature of the reverse iterator. 
 * Technically, rbegin() points to the end() position in the container, which doesn't hold a valid element. 
 * To access valid elements in the container in a reversed manner, the reverse iterator's dereference 
 * operation (`operator*`) is implemented to reference the element one position before the current position.
 * 
 * This implementation ensures that when you start with rbegin(), you are accessing the last valid element 
 * in the container, facilitating the reverse traversal up to the position indicated by rend(), which corresponds 
 * to the first element in the container.
 * 
 * @return Reference to the element one position before the current position in the reverse traversal.
 */
            reference operator*() const {
                _Iterator __tmp = _M_current;
                return *--__tmp;
            }
            
            pointer
            operator->() const { return &(operator*()); }

            reverse_iterator&
            operator++() {
                --_M_current;
                return *this;
            }

            reverse_iterator
            operator++(int) {
                reverse_iterator __tmp = *this;
                --_M_current;
                return __tmp;
            }

            reverse_iterator&
            operator--() {
                ++_M_current;
                return *this;
            }

            reverse_iterator
            operator--(int) {
                reverse_iterator __tmp = *this;
                ++_M_current;
                return __tmp;
            }

/**
 * @brief Adds a given difference to the reverse iterator.
 * 
 * This method essentially performs two operations to return a new reverse iterator that is shifted by a given difference. Here's a breakdown of the sequence of operations:
 * 
 * 1. **Difference Computation**:
 *    Initially, the method computes the new position of the base (original) iterator by subtracting the given difference (`__n`) from the current position (`_M_current`). The subtraction operation invokes the `operator-` on the original iterator (`iterator_type`), which is the base iterator type that the `reverse_iterator` is wrapping.
 * 
 *    ```c++
 *    _M_current - __n
 *    ```
 * 
 * 2. **Reverse Iterator Construction**:
 *    Next, with the newly calculated position, a new `reverse_iterator` object is constructed. This invokes the `reverse_iterator(iterator_type __x)` constructor, where `__x` is the new position computed from the subtraction operation in the first step.
 * 
 *    ```c++
 *    reverse_iterator(_M_current - __n)
 *    ```
 * 
 * As a result, this method returns a new `reverse_iterator` object that points to the new position, effectively shifting the iterator by the specified difference but in the reverse direction.
 * 
 * @param __n The difference to add to the iterator, determining how many positions the iterator should move.
 * @return A new reverse_iterator instance pointing to the new position.
 */
reverse_iterator
operator+(difference_type __n) const {
    return reverse_iterator(_M_current - __n);
}

reverse_iterator&
operator+=(difference_type __n) {
    _M_current -= __n;
    return *this;
}

reverse_iterator
operator-(difference_type __n) const {
    return reverse_iterator(_M_current + __n);
}

reverse_iterator&
operator-=(difference_type __n) {
    _M_current += __n;
    return *this;
}

reference
operator[](difference_type __n) const { return *(*this + __n); }
    };

template<typename _Iterator>
    inline bool
    operator==(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y) {
        return __x.base() == __y.base();
               }
template<typename _Iterator>
    inline bool
    operator<(const reverse_iterator<_Iterator>& __x,
              const reverse_iterator<_Iterator>& __y) {
        return __y.base() < __x.base();
              }
//reuse operator==
template<typename _Iterator>
    inline bool
    operator!=(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y) {
        return !(__x == __y);
               }
//reuse operator<
template<typename _Iterator>
    inline bool
    operator>(const reverse_iterator<_Iterator>& __x,
              const reverse_iterator<_Iterator>& __y) {
        return __y < __x;
              }
//reuse operator<
template<typename _Iterator>
    inline bool
    operator<=(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y) {
        return !(__y < __x);
               }
//reuse operator<   
template<typename _Iterator>
    inline bool
    operator>=(const reverse_iterator<_Iterator>& __x,
               const reverse_iterator<_Iterator>& __y) {
        return !(__x < __y);
               }

template<typename _Iterator>
    inline typename reverse_iterator<_Iterator>::difference_type
    operator-(const reverse_iterator<_Iterator>& __x,
              const reverse_iterator<_Iterator>& __y) {
        return __y.base() - __x.base();
              }
template<typename _Iterator>
    inline reverse_iterator<_Iterator>
    operator+(typename reverse_iterator<_Iterator>::difference_type __n,
              const reverse_iterator<_Iterator>& __x) {
        return reverse_iterator<_Iterator>(__x.base() - __n);
              }
/**
 * The `reverse_iterator` class does not define an operator+ that takes two reverse_iterators as parameters (i.e., reverse_iterator operator+(reverse_iterator)) for several reasons:
 * 
 * 1. **Semantic Ambiguity**:
 *    Adding two iterators does not have a clear, well-defined semantic meaning. It is unclear what the "sum" of two iterators should represent in terms of the traversal of a collection.
 *
 * 2. **Consistency with Standard Library Conventions**:
 *    The C++ Standard Library adopts a convention where the addition operation (`operator+`) is defined between an iterator and an integer value, representing a shift in the position that the iterator points to. This is a well-understood and widely accepted convention. Introducing a new addition operation between two iterators can potentially lead to confusion and inconsistency with existing conventions.
 *
 * 3. **Potential Compatibility Issues**:
 *    Introducing a new `operator+` that accepts two iterators as parameters can lead to compatibility issues with existing code. The STL expects a consistent and well-defined set of operators for iterators. Adding new operators might break compatibility with existing algorithms and code that relies on the established set of iterator operators.
 *
 * Hence, to maintain clarity, consistency, and compatibility, the `reverse_iterator` class adheres to the established conventions for iterator operations, and does not define an addition operation between two reverse_iterators.
 */

/////////////////

//   LegacyOutputIterator == InsertIterator 
// creates a std::insert_iterator of type inferred from the argument
// (function template)
// back_insert_iterator
 
// iterator adaptor for insertion at the end of a container
// (class template)
// front_insert_iterator
 
// iterator adaptor for insertion at the back of a container
template<typename _Container>
    class back_insert_iterator
    //void -> not using the iterator_traits except for the iterator_category
    : public iterator<output_iterator_tag,  void, void, void, void>
    {
    protected:
        _Container* _M_container;
    
    public:
        typedef _Container container_type;

        explicit
        back_insert_iterator(_Container& __x) : _M_container(&__x) {}

        back_insert_iterator&
        operator=(typename _Container::const_reference __value) {
            _M_container->push_back(__value);
            return *this;
        }


// Deciphering back_insert_iterator in C++ STL
// In the C++ Standard Template Library (STL), iterators often adhere to a generalized form, and many STL algorithms operate under the assumption that defined iterator operations (like ++, *, etc.) are available. The back_insert_iterator stands as an output iterator predominantly utilized in output or insertion operations.

// 1. Adhering to Iterator Interface
// To comply with the iterator interface, several operations need to be defined. The operator++, known for moving the iterator to the next position in regular iterators, does not perform any actual operation in the case of back_insert_iterator. This behavior stems from the fact that back_insert_iterator does not represent a position within the actual container; instead, it facilitates operations to insert elements at the end of the container.

// 2. Purpose of Defining operator*
// The operator* is defined for similar reasons; though this operation conventionally returns the current element the iterator points to, in the case of back_insert_iterator, it merely returns itself. This functionality is made possible because it is utilized to conduct insertion operations, making the actual retrieval of an element unnecessary.

// 3. Maintaining Compatibility with STL Algorithms
// Therefore, these operations enable the back_insert_iterator to fulfill the regular iterator interface requirements, thus allowing compatibility with various STL algorithms, even if it does not "move" in the conventional sense. The aim is to facilitate seamless integration with STL algorithms that expect iterators to adhere to a particular interface, thereby promoting code reusability and reducing complexity when dealing with different container types.
        back_insert_iterator&
        operator*() { return *this; }

        back_insert_iterator&
        operator++() { return *this; }

        back_insert_iterator
        operator++(int) { return *this; }
    };
// back_inserter (function template) -> helper function to create back_insert_iterator
    //   std::copy(vec2.begin(), vec2.end(), back_inserter(vec1));

  template<typename _Container>
    inline back_insert_iterator<_Container> 
    back_inserter(_Container& __x) 
    { return back_insert_iterator<_Container>(__x); }

    template<typename _Container>
        class front_insert_iterator
        : public iterator<output_iterator_tag, void, void, void, void>
        {
        protected:
            _Container* _M_container;
        public:
            typedef _Container container_type;

            explicit front_insert_iterator(_Container& __x) : _M_container(&__x) {}

            front_insert_iterator&
            operator=(typename _Container::const_reference __value) {
                _M_container->push_front(__value);
                return *this;
            }

            front_insert_iterator&
            operator*() { return *this; }

            front_insert_iterator&
            operator++() { return *this; }

            front_insert_iterator
            operator++(int) { return *this; }
        };
    template<typename _Container>
        inline front_insert_iterator<_Container>
        front_inserter(_Container& __x) {
            return front_insert_iterator<_Container>(__x);
        }
    
    template<typename _Container>
        class insert_iterator
        : public iterator<output_iterator_tag, void, void, void, void>
        {
        protected:
            _Container* _M_container;
            typename _Container::iterator iter;
        public:
            typedef _Container container_type;

            insert_iterator(_Container& __x, typename _Container::iterator __i)
            : _M_container(&__x), iter(__i) {}

            insert_iterator&
            operator=(typename _Container::const_reference __value) {
                iter = _M_container->insert(iter, __value);
                ++iter;
                return *this;
            }

            insert_iterator&
            operator*() { return *this; }

            insert_iterator&
            operator++() { return *this; }

            insert_iterator&
            operator++(int) { return *this; }
        };

     // std::copy(vec2.begin(), vec2.end(), inserter(vec1, vec1.begin() + 2));
    template<typename _Container, typename _Iterator>
        inline insert_iterator<_Container>
        inserter(_Container& __x, _Iterator __i) {
            return insert_iterator<_Container>(__x, 
            typename _Container::iterator(__i));
        }
    
// Certainly, here's a documentation draft describing __normal_iterator and how it contrasts with other iterator adapters in C++:

// __normal_iterator in C++ Standard Template Library (STL)
// Overview
// The __normal_iterator is a component found within the inner workings of the C++ Standard Template Library (STL). It acts as an internal class utilized by various STL containers to formulate their iterators. Due to its internal and specialized role, it is generally not mentioned or detailed in official documentation or references, making it somewhat elusive to users.

// Characteristics
// Mimicking Standard Pointer Behavior
// The __normal_iterator essentially functions as a wrapper emulating the behavior of standard pointers. It is designed to facilitate the compatibility of ordinary pointers with STL containers. It encompasses several overloaded operators and other beneficial member functions, thereby seamlessly integrating with the various functionalities that STL containers offer.

// General-Purpose Design
// In stark contrast to iterator adapters like back_insert_iterator and insert_iterator, the __normal_iterator is designed with a broader perspective. It fundamentally replicates the interface of pointers without being explicitly associated with a specific container type.

// Contrast with Other Iterator Adapters
// Other iterator adapters, such as back_insert_iterator and insert_iterator, are conceptualized to operate synergistically with designated container types. They essentially act as wrappers that encapsulate specific container operations, thus offering a tailored approach to container interaction and manipulation.

// Conclusion
// In conclusion, while __normal_iterator might not be a prominent figure in the public interface of the STL, its role in the backend facilitates a smoother and more compatible interaction between pointers and STL containers. It stands as a testament to the intricate design and inter-operational capabilities that the STL offers, bridging the gap between traditional pointer operations and modern container functionalities.
template<typename _Iterator, typename _Container>
    class __normal_iterator
        : public iterator<typename iterator_traits<_Iterator>::iterator_category,
        typename iterator_traits<_Iterator>::value_type,
        typename iterator_traits<_Iterator>::difference_type,
        typename iterator_traits<_Iterator>::pointer,
        typename iterator_traits<_Iterator>::reference>
{
protected:
    //this is the iterator that we are wrapping
    //this is the imitate pointer
    //if Tp = built-in type, then pointer = Tp* = _Iterator
    _Iterator _M_current;
public:
    typedef typename iterator_traits<_Iterator>::difference_type difference_type;
    typedef typename iterator_traits<_Iterator>::reference reference;
    typedef typename iterator_traits<_Iterator>::pointer pointer;

    __normal_iterator() : _M_current(_Iterator()) {}

    explicit
    __normal_iterator(const _Iterator& __i) : _M_current(__i) {}

    // Allow iterator to const_iterator conversion
    template<typename _Iter>
    inline __normal_iterator(const __normal_iterator<_Iter, _Container>& __i)
    : _M_current(__i.base()) {}

    // Forward iterator requirements
    reference
    operator*() const { return *_M_current; }

    pointer
    operator->() const { return _M_current; }

    __normal_iterator&
    operator++() {
        ++_M_current;
        return *this;
    }

    __normal_iterator
    operator++(int) {
        return __normal_iterator(_M_current++);
    }

    // Bidirectional iterator requirements
    __normal_iterator&
    operator--() {
        --_M_current;
        return *this;
    }

    __normal_iterator
    operator--(int) {
        return __normal_iterator(_M_current--);
    }

    // Random access iterator requirements
    reference
    operator[](difference_type __n) const { return _M_current[__n]; }

    __normal_iterator&
    operator+=(difference_type __n) {
        _M_current += __n;
        return *this;
    }

    __normal_iterator
    operator+(difference_type __n) const {
        return __normal_iterator(_M_current + __n);
    }

    __normal_iterator&
    operator-=(difference_type __n) {
        _M_current -= __n;
        return *this;
    }

    __normal_iterator
    operator-(difference_type __n) const {
        return __normal_iterator(_M_current - __n);
    }

    difference_type
    operator-(const __normal_iterator& __rhs) const {
        return _M_current - __rhs._M_current;
    }

    const _Iterator&
    base() const { return _M_current; }
};

// Forward iterator requirements
template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator==(const __normal_iterator<_IteratorL, _Container>& __lhs,
               const __normal_iterator<_IteratorR, _Container>& __rhs) {
        return __lhs.base() == __rhs.base();
    }
template<typename _Iterator, typename _Container>
    inline bool
    operator!=(const __normal_iterator<_Iterator, _Container>& __lhs,
               const __normal_iterator<_Iterator, _Container>& __rhs) {
        return !(__lhs == __rhs);
    }
// Random access iterator requirements
template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
              const __normal_iterator<_IteratorR, _Container>& __rhs) {
        return __lhs.base() < __rhs.base();
    }
template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>(const __normal_iterator<_IteratorL, _Container>& __lhs,
              const __normal_iterator<_IteratorR, _Container>& __rhs) {
        return __rhs < __lhs;
    }
template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator<=(const __normal_iterator<_IteratorL, _Container>& __lhs,
               const __normal_iterator<_IteratorR, _Container>& __rhs) {
        return !(__rhs < __lhs);
    }
template<typename _IteratorL, typename _IteratorR, typename _Container>
    inline bool
    operator>=(const __normal_iterator<_IteratorL, _Container>& __lhs,
               const __normal_iterator<_IteratorR, _Container>& __rhs) {
        return !(__lhs < __rhs);
    }

    // ex) 3 + __normal_iterator ->for this case, we need to define the operator+
template<typename _IteratorL, typename _IteratorR, typename _Container>
   inline __normal_iterator<_IteratorL, _Container>
   operator+(typename __normal_iterator<_IteratorL, _Container>::difference_type __n,
             const __normal_iterator<_IteratorR, _Container>& __i) {
        return __normal_iterator<_IteratorL, _Container>(__i.base() + __n);
    }

};
#endif