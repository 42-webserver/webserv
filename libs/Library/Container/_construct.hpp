#ifndef _CONSTRUCT_HPP
# define _CONSTRUCT_HPP

#include <new>
#include "Type.hpp"

namespace ft
{
  /**
   * @maint
   * Constructs an object in existing memory by invoking an allocated
   * object's constructor with an initializer.
   * @endmaint
   */
  template <class _T1, class _T2>
    inline void
    _Construct(_T1* __p, const _T2& __value)
    { new (static_cast<void*>(__p)) _T1(__value); }
  
  /**
   * @maint
   * Constructs an object in existing memory by invoking an allocated
   * object's default constructor (no initializers).
   * @endmaint
   */
  template <class _T1>
    inline void
    _Construct(_T1* __p)
    { new (static_cast<void*>(__p)) _T1(); }

  /**
   * @maint
   * Destroy a range of objects with nontrivial destructors.  
   *
   * This is a helper function used only by _Destroy().
   * @endmaint
   */
  template <class _ForwardIterator>
    inline void
    __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, type::false_type)
    { for ( ; __first != __last; ++__first) _Destroy(&*__first); }

  /**
   * @maint
   * Destroy a range of objects with trivial destructors.  Since the destructors
   * are trivial, there's nothing to do and hopefully this function will be
   * entirely optimized away.
   *
   * This is a helper function used only by _Destroy().
   * @endmaint
   */
  // Trivial destructors(made by compiler) not to do anything.-> so Optimized away.
  template <class _ForwardIterator> 
    inline void
    __destroy_aux(_ForwardIterator, _ForwardIterator, type::true_type)
    { }

  /**
   * @maint
   * Destroy the object pointed to by a pointer type.
   * @endmaint
   */
  template <class _Tp>
    inline void
    _Destroy(_Tp* __pointer)
    { __pointer->~_Tp(); }
  
  /**
   * @maint
   * Destroy a range of objects.  If the value_type of the object has
   * a trivial destructor, the compiler should optimize all of this
   * away, otherwise the objects' destructors must be invoked.
   * @endmaint
   */
  template <class _ForwardIterator>
    inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      typedef typename __type_traits<_Value_type>::has_trivial_destructor
                       _Has_trivial_destructor;

      __destroy_aux(__first, __last, _Has_trivial_destructor());
    }
} // namespace std

#endif /* _CPP_BITS_STL_CONSTRUCT_H */

// Local Variables:
// mode:C++
// End:
