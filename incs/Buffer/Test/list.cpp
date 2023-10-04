#include <iostream>
#include <list>

class a{
public:
	a(){}
	a(const a& other){
		std::cout << "hello?\n";
	}
	void print(){
		std::cout << "hi?\n";
	}
};
int main(void){
	std::list<a> lst;

	lst.push_back(a());
	lst.begin()->print();
	// a* a1;
	// delete a1;
}