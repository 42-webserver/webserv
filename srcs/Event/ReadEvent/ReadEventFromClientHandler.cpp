#include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"
#include "../../../incs/Event/ReadEvent/ReadEventFromClient.hpp"
#include "../../../incs/Event/BufReadHandler.hpp"
#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/FtUtil/ft.hpp"
#include "../../../incs/Server/VirtualServerManager.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../../incs/Event/EventDto/EventDto.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
ReadEventFromClientHandler::ReadEventFromClientHandler(void) : ReadEventHandler(), _HttpRequestParser(ft::make_shared<HttpRequestParser>()) {}
ReadEventFromClientHandler::~ReadEventFromClientHandler(void) {}
const ft::shared_ptr<HttpRequestParser>	&ReadEventFromClientHandler::getHttpRequestParser(void) { return (this->_HttpRequestParser); }
void ReadEventFromClientHandler::handleEvent(Event &event) {
	std::vector<char>	buf;
	BufReadHandler		buf_read_handler(event.getFd(), ft::bufSize);
	
	//check Moudle
	try {
		buf = buf_read_handler.readBuf();
	} catch (const std::exception &e) {
		Logger::getInstance().info("e.what()");
		return ;
	}
	if (buf.empty()) {
		event.offboardQueue();
		return ;
	}

	const RequestParseState state = this->getHttpRequestParser()->parseRequest(buf);


	if (state == FINISH) {
		//fix daegulee :
		ReadEventFromClient *readEventClient = static_cast<ReadEventFromClient*>(&event);
		ft::shared_ptr<VirtualServerManager> virtualServerManager = readEventClient->getVirtualServerManger();
		EventQueue &eventQueue = EventQueue::getInstance();
		EventDto eventDto(readEventClient->getChannel(), virtualServerManager, this->getHttpRequestParser()->getHttpRequest());
		ft::shared_ptr<Event> readEvent = EventFactory::getInstance().createEvent(ft::WRITE_EVENT_TO_CLIENT,
		eventDto);
		readEvent->onboardQueue();
		event.offboardQueue();
	}
}

