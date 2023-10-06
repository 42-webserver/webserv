#include <vector>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>

int main(void){
	int fd = open("test.txt", O_RDONLY);

	std::vector<char> v1(3);
	size_t size = read(fd, v1.data(), 3);
	std::cout << size <<'\n';
	for (std::vector<char>::iterator it = v1.begin();
	it != v1.end(); it++)
		std::cout << *it << '\n';
}