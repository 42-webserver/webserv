#include <iostream>
#include <list>
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/unique_ptr.hpp"

class a{
public:
	a(){
		std::cout << "basic\n";
	}
	a(const a& other){
		std::cout << "copy\n";
	}
};

int main(void){
	std::list<ft::shared_ptr<a> > lst;
	lst.push_back(ft::shared_ptr<a>(new a()));
	// ft::unique_ptr<a> aptr;
	// ft::unique_ptr<a> bptr;
	// aptr = bptr;
}