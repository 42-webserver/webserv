#include "../Assert.hpp"

void foo(){
	// ft::Assert::ScopedAssertChange change(ft::RUNTIME);
	ft::Assert::current_level = ft::RUNTIME;
	ft::Assert::_assert((1< 0), "hi?\n");
}
int main(void){
	foo();
	ft::Assert::_assert((1< 0), "hi?\n");
}