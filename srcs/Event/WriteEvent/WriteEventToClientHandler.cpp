#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"
#include <Http/Exception/HttpException.hpp>
#include <Http/Exception/MethodNotAllowedException.hpp>

WriteEventToClientHandler::WriteEventToClientHandler() : WriteEventHandler() {}
WriteEventToClientHandler::~WriteEventToClientHandler() {}


// clientQueue안에서는 동기화 적으로 작동,
// 파일을 보내는걸 완료했는데 클라이언트가 죽은 경우 고려 
void WriteEventToClientHandler::handleEvent(Event &event){
	WriteEventToClient *curEvent = static_cast<WriteEventToClient *>(&event);
	ft::shared_ptr<Client> client = curEvent->getClient();

	//Todo: check this

	if (client->isRequestEmpty()) {
		if (!client->isResponseEmpty()) {
			ft::shared_ptr<HttpResponse> curResponse = client->getResponse();
			if (curResponse->isSending()){
				if (curResponse->sendToClient(curEvent->getChannel()) == sendingDone)
					client->clearResponseAndRequest();
			}
		}
		else {
			if (client->isClientDie())
				curEvent->offboardQueue();
		}
		return ;
	}

	try {
		ft::shared_ptr<HttpRequest> curRequest = client->getRequest();
		PatternType patternType = client->getPatternType(curEvent->getVirtualServerManger());
		PatternProcessor &patternProcessor = PatternProcessor::getInstance(patternType,
		curEvent->getVirtualServerManger(), client);
		client->allocateResponse();
		
		// Todo: The processor should pre-populate the normalcase buffer with the appropriate format, 
		// excluding the content body, in anticipation of a successful scenario. 
		if (patternProcessor.process() == WAITING) {
			patternProcessor.clear();
		}
		else if (patternProcessor.process() == SUCCESS) {
			patternProcessor.clear();
			ft::shared_ptr<HttpResponse> curResponse = client->getResponse();
			curResponse->setCanSending();
			if (curResponse->sendToClient(curEvent->getChannel()) == sendingDone)
				client->clearResponseAndRequest();
		}
		else {
			//Todo::
		}
	}
	catch (HttpException &e) {
		// Logger::getInstance()->log(LogLevel::ERROR, e.what());
		ErrorPageHandler::getErrorPageResponseTo(client, e.getStatusCode());
		ft::shared_ptr<HttpResponse> response = client->getResponse();
		response->setCanSending();

		if (response->sendToClient(curEvent->getChannel()) == sendingDone)
			client->clearResponseAndRequest();
	}
	catch (std::exception &e) {
		ErrorPageHandler::getErrorPageResponseTo(client, INTERNAL_SERVER_ERROR);
		ft::shared_ptr<HttpResponse> response = client->getResponse();
		response->setCanSending();

		//log error
		if (response->sendToClient(curEvent->getChannel()) == sendingDone)
			client->clearResponseAndRequest();		
	}
}