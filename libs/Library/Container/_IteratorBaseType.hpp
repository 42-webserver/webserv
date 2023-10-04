#ifndef _ITERATORBASETYPE_HPP
# define _ITERATORBASETYPE_HPP
#include <iostream>

namespace ft{
    /**
   *  @defgroup iterator_tags Iterator Tags
   *  These are empty types, used to distinguish different iterators.  The
   *  distinction is not made by what they contain, but simply by what they
   *  are.  Different underlying algorithms can then be used based on the
   *  different operations supporetd by different iterator types.
   *  @{
  */
 typedef long  ptrdiff_t;
 //input iterator
 struct input_iterator_tag {};
    //output iterator
 struct output_iterator_tag {};
 struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    /** @} */  // end of iterator_tags group
 /**
   *  This class does nothing but define nested typedefs.  Iterator classes
   *  can inherit from this class to save some work.  The typedefs are then
   *  used in specializations and overloading.
  */

 //iterator  usage: can be used to iterate through a container (algorithm,
 //container, etc)
 //-> value_type: iteratorValue
//          -> difference_type: type of difference between two iterators
//          -> pointer:pointer to the type pointed by the iterator
//         -> reference: reference to the type pointed by the iterator
//         -> iterator_category: category of the iterator
 template <typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator {
        typedef _Category iterator_category;
        typedef _Tp       value_type;
        typedef _Distance difference_type;
        typedef _Pointer  pointer;
        typedef _Reference reference;
    };
  /**
   *  This class does nothing but define nested typedefs.  The general
   *  version simply "forwards" the nested typedefs from the Iterator
   *  argument.  Specialized versions for pointers and pointers-to-const
   *  provide tighter, more correct semantics.
  */
  //base class for iterator_traits -> if category, value.. are defined, then
  //this class is used
    template <typename _Iterator>
        struct iterator_traits {
            typedef typename _Iterator::iterator_category iterator_category;
            typedef typename _Iterator::value_type        value_type;
            typedef typename _Iterator::difference_type   difference_type;
            typedef typename _Iterator::pointer           pointer;
            typedef typename _Iterator::reference         reference;
        };
    //specialization for pointers (if not find first case, then this one is used)
    template <typename _Tp>
        struct iterator_traits<_Tp*> {
            typedef ft::random_access_iterator_tag iterator_category;
            typedef _Tp                            value_type;
            typedef ft::ptrdiff_t                  difference_type;
            typedef _Tp*                           pointer;
            typedef _Tp&                           reference;
        };
    //specialization for const pointers, (for ConstIterator)
    template <typename _Tp>
        struct iterator_traits<const _Tp*> {
            typedef ft::random_access_iterator_tag iterator_category;
            typedef _Tp                            value_type;
            typedef ft::ptrdiff_t                  difference_type;
            typedef const _Tp*                     pointer;
            typedef const _Tp&                     reference;
        };
    //this class is used to get the iterator_category of an iterator
    // for tag dispatch system
    template <typename _Iter>
        inline typename iterator_traits<_Iter>::iterator_category
        iterator_category(const _Iter&) {
           return typename iterator_traits<_Iter>::iterator_category();
        }
};
#endif