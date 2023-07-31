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
	// TotalReadBuffer -> assign-> copy
	try {
		buf = buf_read_handler.readBuf();
		// std::string str(buf.begin(), buf.end());
		// std::cerr << "buf size : " << buf.size() << " buf : " << str << std::endl;
	} catch (const std::exception &e) {
		Logger::getInstance().info("e.what()");
		return ;
	}
	if (buf.empty()) {
		Logger::getInstance().info("Client Send Eof");
		event.offboardQueue();
		return ;
	}
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
		state = this->getHttpRequestParser()->
		parseRequest(buf, static_cast<ReadEventFromClient*>(&event)->getVirtualServerManger());
		//header -> body  memory

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
		// std::cerr << this->getHttpRequestParser()->getHttpRequest()->getHeader("Host") << std::endl;
		ReadEventFromClient *readEventClient = static_cast<ReadEventFromClient*>(&event);
		ft::shared_ptr<VirtualServerManager> virtualServerManager = readEventClient->getVirtualServerManger();
		// std::cerr << readEventClient->getChannel()->getFd() << std::endl;
		EventDto eventDto(readEventClient->getChannel(), virtualServerManager, this->getHttpRequestParser()->getHttpRequest());
		Event* writeEvent = EventFactory::getInstance().createEvent(ft::WRITE_EVENT_TO_CLIENT,
		eventDto);
		event.offboardQueue();
		writeEvent->onboardQueue();
	}
}

