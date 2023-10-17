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

void ReadEventFromClientHandler::_processReading(ReadEventFromClient *event) {
	try {
		HttpRequestParser &parser = *(this->getHttpRequestParser());
		
		while (parser.parseRequest(event->getVirtualServerManger()) == FINISH)
			event->addRequest(parser.getHttpRequest());

		e_client_event_queue_state state = event->queryClientEventQueueState();
		if (parser.getState() == FINISH && state != Write && state != ReadWrite) {
			//TODO: add to event queue
		}
			
	}
	catch (...) {
		throw ;
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
	try {
		if (readEventClient->canRead() == false)
			return ;
		n = buffer.ioRead(readEventClient->getFd());
		_process(_processMatcher(n), readEventClient);
	} 
	 catch (...) {
		throw ;
	 }
}

