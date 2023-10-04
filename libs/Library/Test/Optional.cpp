#include "../Optional.hpp"
#include <string>
#include <iostream>
#include "../unique_ptr.hpp"
#include "../../shared_ptr/shared_ptr.hpp"
class a{

};
class b : public a{

};
void foo() {
    ft::Optional<int> op1; // 기본 생성
    ft::Optional<int> op2(5); // 값으로 초기화

    if (op1) {
        std::cout << "op1 has value: " << *op1 << std::endl;
    } else {
        std::cout << "op1 does not have a value" << std::endl;
    }

    if (op2) {
        std::cout << "op2 has value: " << *op2 << std::endl;
    } else {
        std::cout << "op2 does not have a value" << std::endl;
    }

    ft::Optional<std::string> opStr("Hello");
    std::cerr << "opStr: " << opStr->size() << " " << *opStr << std::endl;
    opStr = ft::nullopt;
    if (!opStr) {
        std::cout << "opStr reset to no value" << std::endl;
    }
	// std::string hello = *opStr;
	// std::string hello2 = opStr.value();
	std::string hello3 = opStr.value_or("hi?\n");

	std::cout <<  hello3 << '\n';
    ft::Optional<ft::unique_ptr<int> > rx;
    if (!rx.has_value())
        std::cout << "no value\n";
    ft::unique_ptr<int> uptr(new int(42));
    rx = uptr;
    std::cout << *(rx.value()) << '\n';
    // std::cout << *uptr << '\n';

}

void foo1(){
    ft::Optional<ft::unique_ptr<a> > rx;
    ft::unique_ptr<a> k(new b());
    rx = k;
}

// void foo2(){
//     ft::Optional<ft::shared_ptr<a> > rx;
//     rx = ft::shared_ptr<b>();
// }
int main(void){
    foo1();
    system("leaks a.out");
}