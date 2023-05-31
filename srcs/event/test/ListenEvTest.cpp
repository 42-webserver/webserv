#include "../../../incs/event/Event.hpp"
#include "../../../incs/event/ListenEvent.hpp"
#include "../../../incs/EventQueue/EventQueue.hpp"
#include <unistd.h>
#include <fcntl.h>
#include "../../../incs/log/logger.hpp"
// g++ ./test/ListenEvTest.cpp ListenEvent.cpp Event.cpp  ../file/file.cpp ../EventQueue/EventQueue.cpp ../log/logger.cpp -D DEBUG_FLAG

void foo(void)
{
	Logger &log = Logger::getInstance();
	log.debug("start ");
	EventQueue &kq = EventQueue::getInstance();
	ListenEvFactory &factory = ListenEvFactory::getInstance();
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

	// log.info("{} ,123, {}, 1234, {}", 3, "hi", "hi", "hi");
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
	Event *ListenEvent(factory.createEvent(listenfd));
	ListenEvent->onboardQueue();
}
int main(void) {
	foo();

	for (int i = 0; i < 10; i++) {
		EventQueue &EvQueue = EventQueue::getInstance();
		int	event_length;
		// Polling event
		try {
			event_length = EvQueue.pullEvents();
		} catch (const std::exception &e) {
			std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		}
		// Handle event
		for (int i=0; i<event_length; i++) {
			int			event_fd = EvQueue.getEventFd(i);
			Event* event_data = EvQueue.getEventData(i);
			event_data->callEventHandler();
		}
	}
	return (true);
}
