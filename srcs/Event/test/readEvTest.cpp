#include "../../../incs/Event/Event.hpp"
#include "../../../incs/Event/ReadEvent.hpp"
#include "../../../incs/kqueue/Kqueue.hpp"
#include <unistd.h>
#include <fcntl.h>

// g++ ./test/readEvTest.cpp ReadEvent.cpp Event.cpp ../file/file.cpp ../kqueue/Kqueue.cpp 

void foo(void)
{
	ft::shared_ptr<Kqueue> kq = ft::make_shared<Kqueue>(Kqueue());
	ReadEvClientFactory &factory = ReadEvClientFactory::getInstance();
	int fd = open("a", O_RDONLY | O_CREAT, 0644);
		std::cout << fd << '\n';

	ft::unique_ptr<Event> ReadEvClient(factory.createEvent(fd, kq));
	ReadEvClient->callEventHandler();
	std::cout << fd << '\n';
}
int main(void) {
	foo();
	int fd = open("a", O_RDONLY | O_CREAT, 0644);
	std::cout << fd << '\n';
	system("leaks a.out");
}