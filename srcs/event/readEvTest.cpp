#include "../../incs/event/Event.hpp"
#include "../../incs/event/ReadEvent.hpp"
#include <unistd.h>
#include <fcntl.h>
void foo(void)
{
	int fd = open("a", O_RDONLY | O_CREAT);
	ReadEvClientHandler Handler = ReadEvClientHandler();
	ft::unique_ptr<Event> ReadEvClient(new class ReadEvClient(fd, new ReadEvClientHandler()));
	ReadEvClient->callEventHandler();
	std::cout << fd << '\n';
}
int main(void) {
	foo();
		int fd = open("a", O_RDONLY);
std::cout << fd << '\n';
	system("leaks a.out");
}