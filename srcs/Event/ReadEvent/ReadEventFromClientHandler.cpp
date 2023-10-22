#include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"
#include <Event/ReadEvent/ReadEventFromClient.hpp>

ReadEventFromClientHandler::_processFunc ReadEventFromClientHandler::_processFuncs[ConnectionCount] = {
	&ReadEventFromClientHandler::_processReading,
	&ReadEventFromClientHandler::_processNonBlock,
	&ReadEventFromClientHandler::_processClosed
};
ReadEventFromClientHandler::e_client_connection_state	ReadEventFromClientHandler::_processMatcher(size_t n) {
	if (n > 0)
		return (Reading);
	else if (n == 0)
		return (Closed);
	else
		return (NonBlock);
}

//(a b error c d ) -> ( a b error)-> response a, response b, response error
void ReadEventFromClientHandler::_processReading(ReadEventFromClient *event) {
	HttpRequestParser &parser = *(this->getHttpRequestParser());
		
	// while (1) {
	// 		if (parser.parseRequest(event->getVirtualServerManger()) != FINISH)
	// 			break ;
	// 	}
	// 	catch (BadRequestException& e){
	// 		//To do: errorRequest를 만들어서 보내줘야함
	// 	}
	// 	catch (std::exception& e){
	// 		// Logger::getInstance().error(e.what());
	// 		// ErrorPageHandler::getInstance()->process(event->getClient(), INTERNAL_SERVER_ERROR);
	// 		event->offboardQueue();
	// 		return ;
	// 	} 
	// 	event->addRequest(parser.getHttpRequest());
	// }
	while (parser.parseRequest(event->getVirtualServerManger()) != FINISH) {
		event->addRequest(parser.getHttpRequest());
		// Logger::getInstance().info("Parsing");
	}
// parser.parseRequest(event->getVirtualServerManger()) == FINISH
	//To do Error 잡아서 클라이언트에게 errorHttpResponse를 보내줘야함
	if (!event->isRequestEmpty() && !event->isEventQueueTurnOn(Write)) {
		EventFactory& eventFactory = EventFactory::getInstance();
		EventDto eventDto(
			event->getChannel(),
			event->getVirtualServerManger(),
			event->getClient());
		Event *writeEventToClient = eventFactory.createEvent(ft::WRITE_EVENT_TO_CLIENT, eventDto);
		try {
			writeEventToClient->onboardQueue();
		}
		catch(std::exception &e) {
			// Logger::getInstance().error(e.what());
			delete writeEventToClient;
		}
	}
}
void ReadEventFromClientHandler::_processNonBlock(ReadEventFromClient *event) {
	// Logger::getInstance().info("NonBlock");
	(void)event;
}
void ReadEventFromClientHandler::_processClosed(ReadEventFromClient *event) {
	(void)event;
	event->offboardQueue();
}

void ReadEventFromClientHandler::_process(e_client_connection_state state, ReadEventFromClient *event) {
	(this->*_processFuncs[state])(event);
}

ReadEventFromClientHandler::ReadEventFromClientHandler(void) : ReadEventHandler(), _HttpRequestParser(ft::make_shared<HttpRequestParser>()) {}
ReadEventFromClientHandler::~ReadEventFromClientHandler(void) {}
const ft::shared_ptr<HttpRequestParser>	&ReadEventFromClientHandler::getHttpRequestParser(void) { return (this->_HttpRequestParser); }
void ReadEventFromClientHandler::handleEvent(Event &event) {
	IoOnlyReadBuffer& buffer = IoOnlyReadBuffer::getInstance();
	size_t n = 0;
	ReadEventFromClient *readEventClient = static_cast<ReadEventFromClient*>(&event);
	// RequestParseState state;
	//check Moudle
	// TotalReadBuffer -> assign-> copy
	if (readEventClient->isClientDie() == true) {
		readEventClient->offboardQueue();
		return ;
	}
	if (readEventClient->canRead() == false)
		return ;
	n = buffer.ioRead(readEventClient->getFd());
	_process(_processMatcher(n), readEventClient);
	buffer.recycleInstance();
}

