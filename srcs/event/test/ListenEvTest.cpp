#include "../../../incs/event/Event.hpp"
#include "../../../incs/event/ListenEvent.hpp"
#include "../../../incs/kqueue/Kqueue.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "../../../incs/log/logger.hpp"
// g++ ./test/ListenEvTest.cpp ListenEvent.cpp Event.cpp  ../file/file.cpp ../kqueue/Kqueue.cpp ../log/logger.cpp

void foo(void)
{
	Logger &log = Logger::getInstance();
	ft::shared_ptr<Kqueue> kq = ft::make_shared<Kqueue>(Kqueue());
	ListenEvFactory &factory = ListenEvFactory::getInstance();
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
		std::cout << listenfd << '\n';

    int enable = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(4242);
    servaddr.sin_addr.s_addr = htonl(169070146);// 
	bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, 10);

	log.info("Enter Listen Create");
	ft::unique_ptr<Event> ListenEvent(factory.createEvent(listenfd, kq));
	ListenEvent->callEventHandler();
	std::cout << listenfd << '\n';
}
int main(void) {
	foo();
	int fd = open("a", O_RDONLY | O_CREAT, 0644);
	std::cout << fd << '\n';
	system("leaks a.out");
}