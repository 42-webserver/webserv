#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Http/Handler/ErrorPageHandler.hpp"
#include "../../../incs/Http/Response/HttpResponse.hpp"
#include "../../../incs/Log/Logger.hpp"
#include "../../../incs/Event/EventBase/EventFactory.hpp"
#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"
#include <sstream>

WriteEventToClientHandler::WriteEventToClientHandler() : WriteEventHandler() {}
WriteEventToClientHandler::~WriteEventToClientHandler() {}

void WriteEventToClientHandler::handleEvent(Event &event){
		//To do.
	Logger::getInstance().info("WriteEventToClientHandler::handleEvent");
	//Http Req Check -> Question RB ? Req? Multipart, Normal , cgi injection
	// ft::shared_ptr<HttpResponse> response = ErrorPageHandler::getErrorPageResponse(BAD_REQUEST);
	// std::cerr << "WriteEventToClientHandler::handleEvent: " << response->getBody().data() << std::endl;
	

	std::string responseBody = "<html><body><h1>404 Not Found</h1></body></html>";

std::stringstream ss;
ss << "HTTP/1.1 404 Not Found\r\n";
ss << "Date: Sun, 17 Jul 2023 20:05:54 GMT\r\n";
ss << "Server: Apache/2.4.6 (CentOS)\r\n";
ss << "Content-Type: text/html\r\n";
ss << "Content-Length: " << responseBody.size() << "\r\n"; // Response Body의 사이즈를 Content-Length에 추가.
ss << "Connection: Close\r\n\r\n"; // connection을 close로 설정

// Response Body

// Send headers

// Send headers
if (send(event.getFd(), ss.str().data(), ss.str().size(), MSG_NOSIGNAL) == -1) {
	Logger::getInstance().info("Failed to send headers: {}", 
	1,
	strerror(errno));
	event.offboardQueue();
	return ;
}

// Send body
if (send(event.getFd(), responseBody.data(), responseBody.size(), MSG_NOSIGNAL) == -1) {
	Logger::getInstance().info("Failed to send headers: {}", 
	1,
	strerror(errno));
	event.offboardQueue();
	return ;
}
	EventFactory &eventFactory = EventFactory::getInstance();
	WriteEventToClient *writeEventToClient = static_cast<WriteEventToClient*>(&event);
	EventDto eventDto(writeEventToClient->getChannel(), writeEventToClient->getVirtualServerManger());
	Event *readEventFromClient = eventFactory.createEvent(ft::READ_EVENT_FROM_CLIENT, eventDto);
	event.offboardQueue();
	readEventFromClient->onboardQueue();

}