#include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"
#include "../../../incs/Event/ReadEvent/ReadEventFromClient.hpp"
#include "../../../incs/Event/BufReadHandler.hpp"
#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/FtUtil/ft.hpp"
#include "../../../incs/Server/VirtualServerManager.hpp"
#include "../../../incs/Event/EventQueue/EventQueue.hpp"
#include "../../../incs/Event/EventDto/EventDto.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include "../../../incs/Log/Logger.hpp"
ReadEventFromClientHandler::ReadEventFromClientHandler(void) : ReadEventHandler(), _HttpRequestParser(ft::make_shared<HttpRequestParser>()) {}
ReadEventFromClientHandler::~ReadEventFromClientHandler(void) {}
const ft::shared_ptr<HttpRequestParser>	&ReadEventFromClientHandler::getHttpRequestParser(void) { return (this->_HttpRequestParser); }
void ReadEventFromClientHandler::handleEvent(Event &event) {
	std::vector<char>	buf;
	BufReadHandler		buf_read_handler(event.getFd(), ft::bufSize);
	RequestParseState state;
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
	try {
		state = this->getHttpRequestParser()->
		parseRequest(buf, static_cast<ReadEventFromClient*>(&event)->getVirtualServerManger());
	}
	catch (const std::exception &e) {
		//To do : get on Good Control Flow.
		Logger::getInstance().info("e.what()");
		return ;
	}


	if (state == FINISH) {
		//fix daegulee :
		Logger::getInstance().info("FINISH Parse");
		// std::cerr << *(this->getHttpRequestParser()->getHttpRequest().get())
		// << std::endl;
		ReadEventFromClient *readEventClient = static_cast<ReadEventFromClient*>(&event);
		ft::shared_ptr<VirtualServerManager> virtualServerManager = readEventClient->getVirtualServerManger();
		std::cerr << readEventClient->getChannel()->getFd() << std::endl;
		EventDto eventDto(readEventClient->getChannel(), virtualServerManager, this->getHttpRequestParser()->getHttpRequest());
		ft::shared_ptr<Event> writeEvent = EventFactory::getInstance().createEvent(ft::WRITE_EVENT_TO_CLIENT,
		eventDto);
		writeEvent->onboardQueue();
		event.offboardQueue();
	}
}

