#ifndef TYPE_HPP
# define TYPE_HPP

#include <iostream>
namespace ft{

namespace type {

// enable_if    
template<bool B, class T>
struct enable_if {};

template<class T>
struct enable_if<true, T> {
	typedef T type;
};

//integral_constant
    template <class T, T v>
    struct integral_constant {
        static const T value = v;
        typedef T value_type;
        typedef integral_constant<T, v> type;
        operator value_type() const { return value; }
    };

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;
    //integral_constant
   
// is_same
template<typename T, typename U>
struct is_same : public false_type {};

template<typename T>
struct is_same<T, T> : public true_type {};

    //is_integral
    template <typename T>
    struct is_integral : public false_type {};

    template <> struct is_integral<bool> : public true_type {};
    template <> struct is_integral<char> : public true_type {};
    template <> struct is_integral<signed char> : public true_type {};
    template <> struct is_integral<unsigned char> : public true_type {};
    template <> struct is_integral<short> : public true_type {};
    template <> struct is_integral<unsigned short> : public true_type {};
    template <> struct is_integral<int> : public true_type {};
    template <> struct is_integral<unsigned int> : public true_type {};
    template <> struct is_integral<long> : public true_type {};
    template <> struct is_integral<unsigned long> : public true_type {};
    template <> struct is_integral<long long> : public true_type {};
    template <> struct is_integral<unsigned long long> : public true_type {};
    //is_integral
    
    // is_arithmetic
    template <typename T>
    struct is_arithmetic : public false_type {};

    template <> struct is_arithmetic<bool> : public true_type {};
    template <> struct is_arithmetic<char> : public true_type {};
    template <> struct is_arithmetic<signed char> : public true_type {};
    template <> struct is_arithmetic<unsigned char> : public true_type {};
    template <> struct is_arithmetic<short> : public true_type {};
    template <> struct is_arithmetic<unsigned short> : public true_type {};
    template <> struct is_arithmetic<int> : public true_type {};
    template <> struct is_arithmetic<unsigned int> : public true_type {};
    template <> struct is_arithmetic<long> : public true_type {};
    template <> struct is_arithmetic<unsigned long> : public true_type {};
    template <> struct is_arithmetic<long long> : public true_type {};
    template <> struct is_arithmetic<unsigned long long> : public true_type {};

    template <> struct is_arithmetic<float> : public true_type {};
    template <> struct is_arithmetic<double> : public true_type {};
    template <> struct is_arithmetic<long double> : public true_type {};
    //is_arithmetic


// template <typename T, bool isArith>
// struct hasLessHelper;

// template <typename T>
// struct hasLessHelper<T, true> {
//     static const bool value = true;
// };

// template <typename T>
// struct hasLessHelper<T, false> {
// private:
//     typedef char yes[1];
//     typedef char no[2];
//     template <typename U, bool (U::*)(const U&) const>
//     struct SFINAE {};

//     template <typename U>
//     static yes& test(SFINAE<U, &U::operator<>* = 0);
//     static no& test(...);

// public:
//     static const bool value = sizeof(test<T>(0)) == sizeof(yes);
// };

// template <typename T>
// struct hasLess {
//     static const bool value = hasLessHelper<T, is_arithmetic<T>::value>::value;
// };
#define DEFINE_OPERATOR_TRAIT(TRAIT_NAME, OPERATOR, ARITHMETIC_DEFAULT) \
    template <typename T, bool isArith> \
    struct TRAIT_NAME##Helper; \
    \
    template <typename T> \
    struct TRAIT_NAME##Helper<T, true> { \
        static const bool value = ARITHMETIC_DEFAULT; \
    }; \
    \
    template <typename T> \
    struct TRAIT_NAME##Helper<T, false> { \
    private: \
        typedef char yes[1]; \
        typedef char no[2]; \
        template <typename U, bool (U::*)(const U&) const> \
        struct SFINAE {}; \
        \
        template <typename U> \
        static yes& test(SFINAE<U, &U::OPERATOR>* = 0); \
        static no& test(...); \
    \
    public: \
        static const bool value = sizeof(test<T>(0)) == sizeof(yes); \
    }; \
    \
    template <typename T> \
    struct TRAIT_NAME { \
        static const bool value = TRAIT_NAME##Helper<T, is_arithmetic<T>::value>::value; \
    };

    // Define traits for specific operators
    DEFINE_OPERATOR_TRAIT(hasLess, operator<, true)
    DEFINE_OPERATOR_TRAIT(hasEqual, operator==, true)
    DEFINE_OPERATOR_TRAIT(hasGreater, operator==, true)
    DEFINE_OPERATOR_TRAIT(hasNotEqual, operator==, true)
    DEFINE_OPERATOR_TRAIT(hasLessEqual, operator==, true)
    DEFINE_OPERATOR_TRAIT(hasGreaterEqual, operator==, true)

    template <typename T>
    struct isOrdered : public integral_constant<bool, hasLess<T>::value && hasEqual<T>::value> {};

    template <typename T>
    struct isComparison : public integral_constant<bool,
        hasLess<T>::value && hasEqual<T>::value && hasGreater<T>::value &&
        hasNotEqual<T>::value && hasLessEqual<T>::value && hasGreaterEqual<T>::value
    > {};
template <typename T, bool isArith>
struct hasHashFunctionHelper;

template <typename T>
struct hasHashFunctionHelper<T, true> {
    static const bool value = false; // Arithmetic types are assumed not to have hashFunction
};

template <typename T>
struct hasHashFunctionHelper<T, false> {
private:
    typedef char yes[1];
    typedef char no[2];

    template <typename U, size_t (U::*)() const>
    struct SFINAEWithoutParam {};

    template <typename U, size_t (U::*)(int) const>
    struct SFINAEWithParam {};

    template <typename U>
    static yes& test(SFINAEWithoutParam<U, &U::hash>* = 0);
    
    template <typename U>
    static yes& test(SFINAEWithParam<U, &U::hash>* = 0);

    static no& test(...);

public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename T>
struct hasHashFunction {
    static const bool value = hasHashFunctionHelper<T, ft::type::is_arithmetic<T>::value>::value;
};

template <>
struct hasHashFunction<std::string> {
    static const bool value = false;
};
template <typename T>
struct is_std_string : public false_type {};

template <>
struct is_std_string<std::string> : public true_type {};
};

};

namespace ft{
typedef type::true_type __true_type;
typedef type::false_type __false_type;
template <class _Tp>
struct __type_traits { 
   typedef __true_type     this_dummy_member_must_be_first;
                   /* Do not remove this member. It informs a compiler which
                      automatically specializes __type_traits that this
                      __type_traits template is special. It just makes sure that
                      things work if an implementation is using a template
                      called __type_traits for something unrelated. */

   /* The following restrictions should be observed for the sake of
      compilers which automatically produce type specific specializations 
      of this class:
          - You may reorder the members below if you wish
          - You may remove any of the members below if you wish
          - You must not rename members without making the corresponding
            name change in the compiler
          - Members you add will be treated like regular members unless
            you add the appropriate support in the compiler. */
 

   typedef __false_type    has_trivial_default_constructor;
   typedef __false_type    has_trivial_copy_constructor;
   typedef __false_type    has_trivial_assignment_operator;
   typedef __false_type    has_trivial_destructor;
//    struct PODExample {
//     int x;
//     char y;
//     float z;
// };

// struct NonPODExample {
//     int x;
//     std::string y;; <---- string is not a POD type
// };

   //Pod = Plain Old Data <- can be copied with memcpy
   typedef __false_type    is_POD_type;
};


// Provide some specializations.

template<> struct __type_traits<bool> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<char> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<signed char> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned char> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<wchar_t> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<short> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned short> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<int> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned int> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<long long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<unsigned long long> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<float> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<double> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template<> struct __type_traits<long double> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};

template <class _Tp>
struct __type_traits<_Tp*> {
   typedef __true_type    has_trivial_default_constructor;
   typedef __true_type    has_trivial_copy_constructor;
   typedef __true_type    has_trivial_assignment_operator;
   typedef __true_type    has_trivial_destructor;
   typedef __true_type    is_POD_type;
};


// The following could be written in terms of numeric_limits.  
// We're doing it separately to reduce the number of dependencies.

template <class _Tp> struct _Is_integer {
  typedef __false_type _Integral;
};

template<> struct _Is_integer<bool> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<char> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<signed char> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned char> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<wchar_t> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<short> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned short> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<int> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned int> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<long> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned long> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<long long> {
  typedef __true_type _Integral;
};

template<> struct _Is_integer<unsigned long long> {
  typedef __true_type _Integral;
};

template<typename _Tp> struct _Is_normal_iterator {
   typedef __false_type _Normal;
};

// Forward declaration hack, should really include this from somewhere.
namespace ft 
{
  template<typename _Iterator, typename _Container> class __normal_iterator;
}

template<typename _Iterator, typename _Container>
struct _Is_normal_iterator< ft::__normal_iterator<_Iterator, _Container> > {
   typedef __true_type _Normal;
};
};
#endif