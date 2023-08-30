#include <iostream>
#include <list>
#include "../unique_ptr.hpp"
#include "../../shared_ptr/shared_ptr.hpp"

void foo(){
	std::list<ft::unique_ptr<int> > lst;

	ft::unique_ptr<int> x(new int(42));
	lst.push_back(x.transfer());
}
class a{

};
class b : public a{

};
void foo2(){
	// ft::unique_ptr<a> x;
	ft::shared_ptr<a> x;
}
int main(){
	// foo();
	system("leaks a.out");
}