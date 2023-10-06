#include <iostream>
#include <vector>

class test{
private:
	std::vector<char> _hi;
public:
	test() : _hi(10000){
		std::cout << "test\n";
	}
};

int main(void){
	std::cout << sizeof(test);
}