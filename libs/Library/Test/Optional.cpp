#include "../optional.hpp"
#include <string>
#include <iostream>

int main() {
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

    return 0;
}
