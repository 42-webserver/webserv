#include "../../../incs/Event/ReadEvent/ReadEventFromClientHandler.hpp"
#include "../../../incs/Event/BufReadHandler.hpp"
#include "../../../incs/Event/ReadEvent/ReadEvent.hpp"
#include "../../../incs/FtUtil/ft.hpp"

ReadEventFromClientHandler::ReadEventFromClientHandler(void) : _HttpRequestParser(ft::make_shared<HttpRequestParser>()) {}
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

	// const RequestParseState state = this->getHttpRequestParser()->parseRequest(buf);


	// if (state == FINISH) {
		// // To do : check Host and Select virtual server
		// findHandler 
		// ReadEventClient *readEventClient = dynamic_cast<ReadEventClient*>(&event);
		// std::string host =  this->getHttpRequestParser()->getHttpRequest().getHost();
		// readEventClient->getPhysicalServer()->findVirtualServer(host);
		//
		// // To do : prepareResponse->cgi, client 

		// //
		// // To do : Add Appropriate Event
	// }
}

