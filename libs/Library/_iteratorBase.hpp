#ifndef _ITERATORBASE_HPP
# define _ITERATORBASE_HPP

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
 /// Making input iterator
 struct input_iterator_tag {};
 /// Making output iterator
 struct output_iterator_tag {};
 /// Forward iterator support a superset of input iterator operations
 struct forward_itertator_tag : public input_iterator_tag {};
    /// Bidirectional iterators support a superset of forward iterator
 struct bidirectional_iterator_tag : public forward_itertator_tag {};
    /// Random-access iterators support a superset of bidirectional iterator
 struct random_access_iterator_tag : public bidirectional_iterator_tag {};
 //@}

  /**
   *  This class does nothing but define nested typedefs.  Iterator classes
   *  can inherit from this class to save some work.  The typedefs are then
   *  used in specializations and overloading.
  */
 template <typename _Category, typename _Tp, typename _Distance = std::ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct iterator {
        typedef _Category  iterator_category;

        typedef _Tp        value_type;

        typedef _Distance  difference_type;

        typedef _Pointer   pointer;

        typedef _Reference reference;
    };
    template <typename _Iterator>
        struct iterator_traits {
            typedef typename _Iterator::iterator_category iterator_category;
            typedef typename _Iterator::value_type        value_type;
            typedef typename _Iterator::difference_type   difference_type;
            typedef typename _Iterator::pointer           pointer;
            typedef typename _Iterator::reference         reference;
        };
    template <typename _Tp>
        struct iterator_traits<_Tp*> {
            typedef random_access_iterator_tag iterator_category;
            typedef _Tp                         value_type;
            typedef std::ptrdiff_t              difference_type;
            typedef _Tp*                        pointer;
            typedef _Tp&                        reference;
        };
    template <typename _Tp>
        struct iterator_traits<const _Tp*> {
            typedef random_access_iterator_tag iterator_category;
            typedef _Tp                         value_type;
            typedef std::ptrdiff_t              difference_type;
            typedef const _Tp*                  pointer;
            typedef const _Tp&                  reference;
        };
    template <typename _Iter>
        inline typename iterator_traits<_Iter>::iterator_category
        __iterator_category(const _Iter&)
        { return typename iterator_traits<_Iter>::iterator_category(); }
};



#endif