#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"
#include <Http/Exception/HttpException.hpp>

WriteEventToClientHandler::WriteEventToClientHandler() : WriteEventHandler() {}
WriteEventToClientHandler::~WriteEventToClientHandler() {}


// clientQueue안에서는 동기화 적으로 작동, 
void WriteEventToClientHandler::handleEvent(Event &event){
	WriteEventToClient *curEvent = static_cast<WriteEventToClient *>(&event);
	ft::shared_ptr<Client> client = curEvent->getClient();

	//Todo: check this
	if (client->isRequestEmpty() && client->isResponseEmpty()) {
		if (client->isClientDie()) {
			curEvent->offboardQueue();
		}
		return ;
	}

	try {
		ft::shared_ptr<HttpRequest> curRequest = client->getRequest();
		PatternType patternType = client->getPatternType(curEvent->getVirtualServerManger());
		PatternProcessor &patternProcessor = PatternProcessor::getInstance(patternType,
		curEvent->getVirtualServerManger(), client);
		patternProcessor.process();
		patternProcessor.clear();
	}
	catch (HttpException &e) {
		// Logger::getInstance()->log(LogLevel::ERROR, e.what());
		// ErrorPageHandler::getInstance()->process(client, e.getStatusCode());
	}
	catch (std::exception &e) {
		// Logger::getInstance()->log(LogLevel::ERROR, e.what());
		// ErrorPageHandler::getInstance()->process(client, INTERNAL_SERVER_ERROR);
	}
	catch (...) {
		// Logger::getInstance()->log(LogLevel::ERROR, "Unknown Error");
		// ErrorPageHandler::getInstance()->process(client, INTERNAL_SERVER_ERROR);
	}
}