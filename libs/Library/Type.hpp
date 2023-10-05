// #ifndef TYPE_HPP
// # define TYPE_HPP

// #include <type_traits>
// namespace ft{

// namespace type {

// // enable_if    
// template<bool B, class T>
// struct enable_if {};

// template<class T>
// struct enable_if<true, T> {
// 	typedef T type;
// };

// //integral_constant
//     template <class T, T v>
//     struct integral_constant {
//         static const T value = v;
//         typedef T value_type;
//         typedef integral_constant<T, v> type;
//         operator value_type() const { return value; }
//     };

//     typedef integral_constant<bool, true> true_type;
//     typedef integral_constant<bool, false> false_type;
//     //integral_constant

//     //is_integral
//     template <typename T>
//     struct is_integral : public false_type {};

//     template <> struct is_integral<bool> : public true_type {};
//     template <> struct is_integral<char> : public true_type {};
//     template <> struct is_integral<signed char> : public true_type {};
//     template <> struct is_integral<unsigned char> : public true_type {};
//     template <> struct is_integral<short> : public true_type {};
//     template <> struct is_integral<unsigned short> : public true_type {};
//     template <> struct is_integral<int> : public true_type {};
//     template <> struct is_integral<unsigned int> : public true_type {};
//     template <> struct is_integral<long> : public true_type {};
//     template <> struct is_integral<unsigned long> : public true_type {};
//     template <> struct is_integral<long long> : public true_type {};
//     template <> struct is_integral<unsigned long long> : public true_type {};
//     //is_integral
    
//     // is_arithmetic
//     template <typename T>
//     struct is_arithmetic : public false_type {};

//     template <> struct is_arithmetic<bool> : public true_type {};
//     template <> struct is_arithmetic<char> : public true_type {};
//     template <> struct is_arithmetic<signed char> : public true_type {};
//     template <> struct is_arithmetic<unsigned char> : public true_type {};
//     template <> struct is_arithmetic<short> : public true_type {};
//     template <> struct is_arithmetic<unsigned short> : public true_type {};
//     template <> struct is_arithmetic<int> : public true_type {};
//     template <> struct is_arithmetic<unsigned int> : public true_type {};
//     template <> struct is_arithmetic<long> : public true_type {};
//     template <> struct is_arithmetic<unsigned long> : public true_type {};
//     template <> struct is_arithmetic<long long> : public true_type {};
//     template <> struct is_arithmetic<unsigned long long> : public true_type {};

//     template <> struct is_arithmetic<float> : public true_type {};
//     template <> struct is_arithmetic<double> : public true_type {};
//     template <> struct is_arithmetic<long double> : public true_type {};
//     //is_arithmetic


// // template <typename T, bool isArith>
// // struct hasLessHelper;

// // template <typename T>
// // struct hasLessHelper<T, true> {
// //     static const bool value = true;
// // };

// // template <typename T>
// // struct hasLessHelper<T, false> {
// // private:
// //     typedef char yes[1];
// //     typedef char no[2];
// //     template <typename U, bool (U::*)(const U&) const>
// //     struct SFINAE {};

// //     template <typename U>
// //     static yes& test(SFINAE<U, &U::operator<>* = 0);
// //     static no& test(...);

// // public:
// //     static const bool value = sizeof(test<T>(0)) == sizeof(yes);
// // };

// // template <typename T>
// // struct hasLess {
// //     static const bool value = hasLessHelper<T, is_arithmetic<T>::value>::value;
// // };
// #define DEFINE_OPERATOR_TRAIT(TRAIT_NAME, OPERATOR, ARITHMETIC_DEFAULT) \
//     template <typename T, bool isArith> \
//     struct TRAIT_NAME##Helper; \
//     \
//     template <typename T> \
//     struct TRAIT_NAME##Helper<T, true> { \
//         static const bool value = ARITHMETIC_DEFAULT; \
//     }; \
//     \
//     template <typename T> \
//     struct TRAIT_NAME##Helper<T, false> { \
//     private: \
//         typedef char yes[1]; \
//         typedef char no[2]; \
//         template <typename U, bool (U::*)(const U&) const> \
//         struct SFINAE {}; \
//         \
//         template <typename U> \
//         static yes& test(SFINAE<U, &U::OPERATOR>* = 0); \
//         static no& test(...); \
//     \
//     public: \
//         static const bool value = sizeof(test<T>(0)) == sizeof(yes); \
//     }; \
//     \
//     template <typename T> \
//     struct TRAIT_NAME { \
//         static const bool value = TRAIT_NAME##Helper<T, is_arithmetic<T>::value>::value; \
//     };

//     // Define traits for specific operators
//     DEFINE_OPERATOR_TRAIT(hasLess, operator<, true)
//     DEFINE_OPERATOR_TRAIT(hasEqual, operator==, true)
//     DEFINE_OPERATOR_TRAIT(hasGreater, operator==, true)
//     DEFINE_OPERATOR_TRAIT(hasNotEqual, operator==, true)
//     DEFINE_OPERATOR_TRAIT(hasLessEqual, operator==, true)
//     DEFINE_OPERATOR_TRAIT(hasGreaterEqual, operator==, true)

//     template <typename T>
//     struct isOrdered : public integral_constant<bool, hasLess<T>::value && hasEqual<T>::value> {};

//     template <typename T>
//     struct isComparison : public integral_constant<bool,
//         hasLess<T>::value && hasEqual<T>::value && hasGreater<T>::value &&
//         hasNotEqual<T>::value && hasLessEqual<T>::value && hasGreaterEqual<T>::value
//     > {};
// template <typename T, bool isArith>
// struct hasHashFunctionHelper;

// template <typename T>
// struct hasHashFunctionHelper<T, true> {
//     static const bool value = false; // Arithmetic types are assumed not to have hashFunction
// };

// template <typename T>
// struct hasHashFunctionHelper<T, false> {
// private:
//     typedef char yes[1];
//     typedef char no[2];

//     template <typename U, size_t (U::*)() const>
//     struct SFINAEWithoutParam {};

//     template <typename U, size_t (U::*)(int) const>
//     struct SFINAEWithParam {};

//     template <typename U>
//     static yes& test(SFINAEWithoutParam<U, &U::hash>* = 0);
    
//     template <typename U>
//     static yes& test(SFINAEWithParam<U, &U::hash>* = 0);

//     static no& test(...);

// public:
//     static const bool value = sizeof(test<T>(0)) == sizeof(yes);
// };

// template <typename T>
// struct hasHashFunction {
//     static const bool value = hasHashFunctionHelper<T, ft::type::is_arithmetic<T>::value>::value;
// };

// template <>
// struct hasHashFunction<std::string> {
//     static const bool value = false;
// };
// template <typename T>
// struct is_std_string : public false_type {};

// template <>
// struct is_std_string<std::string> : public true_type {};
// };
// }
// #endif