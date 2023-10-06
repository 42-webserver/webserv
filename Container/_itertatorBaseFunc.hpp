#ifndef _ITERATORBASEFUNC_HPP
# define _ITERATORBASEFUNC_HPP

#include "_iteratorBaseType.hpp"

namespace ft{
    // _distance
    // 3th param is the tag dispatch system
    // bidirectional_iterator_tag, forward_iterator_tag == same input_iterator_tag for distance
    template<typename _InputIterator>
        inline typename iterator_traits<_InputIterator>::difference_type
        __distance(_InputIterator __first, _InputIterator __last,
                  input_iterator_tag) { 
                    //concept check
                    // __ftcpp_function_requires(_InputIteratorConcept<_InputIterator>)
                    typename iterator_traits<_InputIterator>::difference_type __n = 0;
                    while (__first != __last) {
                        //pre-increment is more efficient than post-increment
                        ++__first;
                        ++__n;
                    }
                    return __n;
        }
    template<typename _RandomAccessIterator>
        inline typename iterator_traits<_RandomAccessIterator>::difference_type
        __distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
                  random_access_iterator_tag) {
                    //concept check
                    // __ftcpp_function_requires(_RandomAccessIteratorConcept<_RandomAccessIterator>)
                    return __last - __first;
        }
    template<typename _InputIterator>
        inline typename iterator_traits<_InputIterator>::difference_type
        distance(_InputIterator __first, _InputIterator __last) {
            //concept check
            // tag dispatch system
            return __distance(__first, __last, iterator_category(__first));
        }
    // _advance
    template<typename _InputIterator, typename _Distance>
        inline void
        __advance(_InputIterator& __i, _Distance __n, input_iterator_tag) {
            //concept check
            // __ftcpp_function_requires(_InputIteratorConcept<_InputIterator>)
            while (__n--) {
                ++__i;
            }
        }
    template<typename _BidirectionalIterator, typename _Distance>
        inline void
        __advance(_BidirectionalIterator& __i, _Distance __n, bidirectional_iterator_tag) {
            //concept check
            // __ftcpp_function_requires(_BidirectionalIteratorConcept<_BidirectionalIterator>)
            if (__n >= 0) {
                while (__n--) {
                    ++__i;
                }
            } else {
                while (__n++) {
                    --__i;
                }
            }
        }
    template<typename _RandomAccessIterator, typename _Distance>
        inline void
        __advance(_RandomAccessIterator& __i, _Distance __n, random_access_iterator_tag) {
            //concept check
            // __ftcpp_function_requires(_RandomAccessIteratorConcept<_RandomAccessIterator>)
            __i += __n;
        }
    template<typename _InputIterator, typename _Distance>
        inline void
        advance(_InputIterator& __i, _Distance __n) {
            //concept check
            // tag dispatch system
            __advance(__i, __n, iterator_category(__i));
        }
};
#endif