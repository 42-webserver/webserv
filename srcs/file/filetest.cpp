#include "../../incs/file/file.hpp"
#include "../../libs/shared_ptr/incs/shared_ptr.hpp"
#include <fcntl.h>

void foo() {
	int fd = open("a", O_CREAT|O_RDONLY);	
	ft::shared_ptr<FdClass> fclass(new FdClass(fd));
	std::cout << fd << '\n';
	ft::shared_ptr<FdClass> another = fclass;
	std::cout<< another->getFd() << '\n'; 
}

int main() {
	foo();
	int fd = open("a", O_RDONLY);	
	std::cout << fd << '\n';
	system("leaks a.out");
	return 0;
}
