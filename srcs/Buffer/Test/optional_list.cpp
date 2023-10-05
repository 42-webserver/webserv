#include <iostream>
#include <list>
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../../libs/Library/unique_ptr.hpp"
#include "../../../libs/Library/Optional.hpp"
class a{
private:
	int _a;
public:
	a() : _a(5){
		std::cout << "basic\n";
	}
	a(const a& other) : _a(other._a){
		std::cout << "copy\n";
	}
};

int main(void){
	std::list<ft::shared_ptr<a> > _lst;

	_lst.push_back(ft::shared_ptr<a>(new a()));
	// ft::unique_ptr<a> aptr;
	// ft::unique_ptr<a> bptr;
	// aptr = bptr;
}