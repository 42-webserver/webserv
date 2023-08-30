#include "../unique_ptr.hpp" 
#include <iostream>

struct c{
	int a;
};

class x{

};
class d : public x{
	
};
void foo(){
	ft::unique_ptr<int> a(new int(42));
	std::cout << *a << '\n';
	ft::unique_ptr<c> cx(new c());
	cx->a = 0;
	ft::unique_ptr<int> r;
	std::cout << cx->a << '\n';
	std::cout << *(a.get()) << '\n';
	// std::cout << *r << '\n';
	a.swap(r);
	// std::cout << *a << '\n';
		std::cout << *r << '\n';
	ft::unique_ptr<x> p(new d());

}
int main(void ){
	foo();
	system("leaks a.out");
}