#include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"

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
void ReadEventFromClientHandler::_processReading() {
	try {
		// while bufBlocksize end
		// 1 - 2 - 3 - 4 - 5 -6 -7 -8 <-Total
		// Read 1  char*- 2 - 3 - 4 - 5 -6 -7 -8 copy
		// list<vector char> 
		/// 1 -> vecor<char>

		//ft:shred<request > tmp = this->getHttpRequestParser()->getHttpRequest();
		// this->_httpReauest = new shared();
		// return tmp;
		// vector <char>  [ req 1 re2 req3]
		// state = this->getHttpRequestParser()->
		// parseRequest(buf, static_cast<ReadEventFromClient*>(&event)->getVirtualServerManger());
		//header -> body  memory

	}
	catch (const std::exception &e) {
		//To do : get on Good Control Flow.
		Logger::getInstance().info("e.what()");
		return ;
	}


	// if (state == FINISH) {
		//fix daegulee :
		Logger::getInstance().info("FINISH Parse");
		// std::cerr << *(this->getHttpRequestParser()->getHttpRequest().get())
		// << std::endl;
		// std::cerr << this->getHttpRequestParser()->getHttpRequest()->getHeader("Host") << std::endl;
		ReadEventFromClient *readEventClient = static_cast<ReadEventFromClient*>(&event);
		ft::shared_ptr<VirtualServerManager> virtualServerManager = readEventClient->getVirtualServerManger();
		// std::cerr << readEventClient->getChannel()->getFd() << std::endl;
		EventDto eventDto(readEventClient->getChannel(), virtualServerManager, this->getHttpRequestParser()->getHttpRequest());
		Event* writeEvent = EventFactory::getInstance().createEvent(ft::WRITE_EVENT_TO_CLIENT,
		eventDto);
		event.offboardQueue();
		writeEvent->onboardQueue();
	// }
}
void ReadEventFromClientHandler::_processNonBlock() {
	// Logger::getInstance().info("NonBlock");
}
void ReadEventFromClientHandler::_processClosed() {
	//To do : client Close Connection
	// Logger::getInstance().info("Closed");
}

void ReadEventFromClientHandler::_process(e_client_connection_state state) {
	(this->*_processFuncs[state])();
}

ReadEventFromClientHandler::ReadEventFromClientHandler(void) : ReadEventHandler(), _HttpRequestParser(ft::make_shared<HttpRequestParser>()) {}
ReadEventFromClientHandler::~ReadEventFromClientHandler(void) {}
const ft::shared_ptr<HttpRequestParser>	&ReadEventFromClientHandler::getHttpRequestParser(void) { return (this->_HttpRequestParser); }
void ReadEventFromClientHandler::handleEvent(Event &event) {
	IoOnlyReadBuffer& buffer = IoOnlyReadBuffer::getInstance();
	size_t n = 0;
	// RequestParseState state;
	//check Moudle
	// TotalReadBuffer -> assign-> copy
	try {
		n = buffer.ioRead(static_cast<ReadEventFromClient*>(&event)->getChannel()->getFd()); 
		_process(_processMatcher(n));
	} 
	 catch (...) {
		throw ;
	 }
}

