#include "../Type.hpp"
#include "../Assert.hpp"

template <typename T>
class MyClass {
public:
    MyClass() {
		ft::Assert::current_level = ft::Assert::COMPILE;
		ASSERT(ft::type::hasLess<T>::value, "Type_does_not_have_less_than_operator");
	}
};

struct A {
    bool operator<(const A&) const { return true; }
};

struct B {};

template<typename T>
struct C
{
	C() {
		ft::Assert::current_level = ft::Assert::COMPILE;
		ASSERT(ft::type::isOrdered<T>::value, "Type_does_not_have_less_than_operator");
	}
};


int main() {
    // MyClass<A> objA;  // OK
    // MyClass<B> objB;  // This should cause a compile error
	// C<int> c;
	MyClass<B> objInt;
	// std::cout << ft::type::is_arithmetic<int>::value << '\n';
    return 0;
}
