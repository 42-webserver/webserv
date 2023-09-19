// #ifndef _THREAD_HPP
// # define _THREAD_HPP
// #include <iostream>

// namespace ft
// {
  
// typedef pthread_t __gthread_t;
// typedef pthread_key_t __gthread_key_t;
// typedef pthread_once_t __gthread_once_t;
// typedef pthread_mutex_t __gthread_mutex_t;

// static inline int
// __gthread_mutex_lock (__gthread_mutex_t *__mutex)
// {
//   if (__tpf_pthread_active ())
//     return __gthrw_(pthread_mutex_lock) (__mutex);
//   else
//     return 0;
// }
// static inline int
// __gthread_mutex_unlock (__gthread_mutex_t *__mutex)
// {
//   if (__tpf_pthread_active ())
//     return __gthrw_(pthread_mutex_unlock) (__mutex);
//   else
//     return 0;
// }
//   // Class _Refcount_Base provides a type, _RC_t, a data member,
//   // _M_ref_count, and member functions _M_incr and _M_decr, which perform
//   // atomic preincrement/predecrement.  The constructor initializes 
//   // _M_ref_count.
//   struct _Refcount_Base
//   {
//     // The type _RC_t
//     typedef size_t _RC_t;
    
//     // The data member _M_ref_count
//     volatile _RC_t _M_ref_count;
    
//     // Constructor
//     __gthread_mutex_t _M_ref_count_lock;

//     _Refcount_Base(_RC_t __n) : _M_ref_count(__n)
//     {
// #ifdef __GTHREAD_MUTEX_INIT
//       __gthread_mutex_t __tmp = __GTHREAD_MUTEX_INIT;
//       _M_ref_count_lock = __tmp;
// #elif defined(__GTHREAD_MUTEX_INIT_FUNCTION)
//       __GTHREAD_MUTEX_INIT_FUNCTION (&_M_ref_count_lock);
// #else
// #error __GTHREAD_MUTEX_INIT orz __GTHREAD_MUTEX_INIT_FUNCTION should be defined by gthr.h abstraction layer, report problem to libstdc++@gcc.gnu.org.
// #endif
//     }

//     void 
//     _M_incr() 
//     {
//       __gthread_mutex_lock(&_M_ref_count_lock);
//       ++_M_ref_count;
//       __gthread_mutex_unlock(&_M_ref_count_lock);
//     }

//     _RC_t 
//     _M_decr() 
//     {
//       __gthread_mutex_lock(&_M_ref_count_lock);
//       volatile _RC_t __tmp = --_M_ref_count;
//       __gthread_mutex_unlock(&_M_ref_count_lock);
//       return __tmp;
//     }
//   };

//   // Atomic swap on unsigned long
//   // This is guaranteed to behave as though it were atomic only if all
//   // possibly concurrent updates use _Atomic_swap.
//   // In some cases the operation is emulated with a lock.
// #if defined (__GTHREAD_MUTEX_INIT)
//   // This could be optimized to use the atomicity.h abstraction layer.
//   // vyzo: simple _Atomic_swap implementation following the guidelines above
//   // We use a template here only to get a unique initialized instance.
//   template<int __dummy>
//     struct _Swap_lock_struct 
//     { static __gthread_mutex_t _S_swap_lock; };

//   template<int __dummy>
//     __gthread_mutex_t
//     _Swap_lock_struct<__dummy>::_S_swap_lock = __GTHREAD_MUTEX_INIT;

//   // This should be portable, but performance is expected to be quite
//   // awful.  This really needs platform specific code.
//   inline unsigned long 
//   _Atomic_swap(unsigned long * __p, unsigned long __q) 
//   {
//     __gthread_mutex_lock(&_Swap_lock_struct<0>::_S_swap_lock);
//     unsigned long __result = *__p;
//     *__p = __q;
//     __gthread_mutex_unlock(&_Swap_lock_struct<0>::_S_swap_lock);
//     return __result;
//   }
// #endif

//   // Locking class.  Note that this class *does not have a
//   // constructor*.  It must be initialized either statically, with
//   // __STL_MUTEX_INITIALIZER, or dynamically, by explicitly calling
//   // the _M_initialize member function.  (This is similar to the ways
//   // that a pthreads mutex can be initialized.)  There are explicit
//   // member functions for acquiring and releasing the lock.

//   // There is no constructor because static initialization is
//   // essential for some uses, and only a class aggregate (see section
//   // 8.5.1 of the C++ standard) can be initialized that way.  That
//   // means we must have no constructors, no base classes, no virtual
//   // functions, and no private or protected members.

// #if !defined(__GTHREAD_MUTEX_INIT) && defined(__GTHREAD_MUTEX_INIT_FUNCTION)
//   extern __gthread_mutex_t _GLIBCPP_mutex;
//   extern __gthread_mutex_t *_GLIBCPP_mutex_address;
//   extern __gthread_once_t _GLIBCPP_once;
//   extern void _GLIBCPP_mutex_init (void);
//   extern void _GLIBCPP_mutex_address_init (void);
// #endif

//   struct _STL_mutex_lock
//   {
//     // The class must be statically initialized with __STL_MUTEX_INITIALIZER.
// #if !defined(__GTHREAD_MUTEX_INIT) && defined(__GTHREAD_MUTEX_INIT_FUNCTION)
//     volatile int _M_init_flag;
//     __gthread_once_t _M_once;
// #endif
//     __gthread_mutex_t _M_lock;

//     void 
//     _M_initialize() 
//     {
// #ifdef __GTHREAD_MUTEX_INIT
//       // There should be no code in this path given the usage rules above.
// #elif defined(__GTHREAD_MUTEX_INIT_FUNCTION)
//       if (_M_init_flag) return;
//       if (__gthread_once (&_GLIBCPP_once, _GLIBCPP_mutex_init) != 0
// 	  && __gthread_active_p ())
// 	abort ();
//       __gthread_mutex_lock (&_GLIBCPP_mutex);
//       if (!_M_init_flag) 
// 	{
// 	  // Even though we have a global lock, we use __gthread_once to be
// 	  // absolutely certain the _M_lock mutex is only initialized once on
// 	  // multiprocessor systems.
// 	  _GLIBCPP_mutex_address = &_M_lock;
// 	  if (__gthread_once (&_M_once, _GLIBCPP_mutex_address_init) != 0
// 	    && __gthread_active_p ())
// 	    abort ();
// 	  _M_init_flag = 1;
// 	}
//       __gthread_mutex_unlock (&_GLIBCPP_mutex);
// #endif
//     }

//     void 
//     _M_acquire_lock() 
//     {
// #if !defined(__GTHREAD_MUTEX_INIT) && defined(__GTHREAD_MUTEX_INIT_FUNCTION)
//       if (!_M_init_flag) _M_initialize();
// #endif
//       __gthread_mutex_lock(&_M_lock);
//     }

//     void 
//     _M_release_lock() 
//     {
// #if !defined(__GTHREAD_MUTEX_INIT) && defined(__GTHREAD_MUTEX_INIT_FUNCTION)
//       if (!_M_init_flag) _M_initialize();
// #endif
//       __gthread_mutex_unlock(&_M_lock);
//     }
//   };
  
// #ifdef __GTHREAD_MUTEX_INIT
// #define __STL_MUTEX_INITIALIZER = { __GTHREAD_MUTEX_INIT }
// #elif defined(__GTHREAD_MUTEX_INIT_FUNCTION)
// #ifdef __GTHREAD_MUTEX_INIT_DEFAULT
// #define __STL_MUTEX_INITIALIZER \
//   = { 0, __GTHREAD_ONCE_INIT, __GTHREAD_MUTEX_INIT_DEFAULT }
// #else
// #define __STL_MUTEX_INITIALIZER = { 0, __GTHREAD_ONCE_INIT }
// #endif
// #endif

//   // A locking class that uses _STL_mutex_lock.  The constructor takes a
//   // reference to an _STL_mutex_lock, and acquires a lock.  The
//   // destructor releases the lock.  It's not clear that this is exactly
//   // the right functionality.  It will probably change in the future.
//   struct _STL_auto_lock
//   {
//     _STL_mutex_lock& _M_lock;
    
//     _STL_auto_lock(_STL_mutex_lock& __lock) : _M_lock(__lock)
//     { _M_lock._M_acquire_lock(); }

//     ~_STL_auto_lock() { _M_lock._M_release_lock(); }

//   private:
//     void operator=(const _STL_auto_lock&);
//     _STL_auto_lock(const _STL_auto_lock&);
//   };
  
// } // namespace std

// #endif 