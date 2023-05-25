#include "./singleTon.hpp"

template <typename T>
ft::unique_ptr<T> Singleton<T>::instance(NULL);

//usage----

// class MyClass : public Singleton<MyClass> {
//     friend class Singleton<MyClass>;

// public:
//     void someFunction() {
//         std::cout<< "hi\n";
//     }

//     // ... other methods here ...
// };

// int main(void) {
// 	MyClass& instance = MyClass::getInstance();
// 	instance.someFunction();

// }
//usage--