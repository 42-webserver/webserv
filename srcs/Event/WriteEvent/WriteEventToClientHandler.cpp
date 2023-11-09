#include "../../../incs/Event/WriteEvent/WriteEventToClientHandler.hpp"
#include "../../../incs/Event/WriteEvent/WriteEventToClient.hpp"
#include <Http/Exception/HttpException.hpp>
#include <Http/Exception/MethodNotAllowedException.hpp>
#include <Event/EventQueue/EventQueue.hpp>
WriteEventToClientHandler::WriteEventToClientHandler() : WriteEventHandler() {}
WriteEventToClientHandler::~WriteEventToClientHandler() {}



// clientQueue안에서는 동기화 적으로 작동,
// 파일을 보내는걸 완료했는데 클라이언트가 죽은 경우 고려 
void WriteEventToClientHandler::_partialSending(ft::shared_ptr<HttpResponse> response, ft::shared_ptr<Client> client,
WriteEventToClient *curEvent){
	e_send_To_client_status sendingStatus = response->sendToClient(curEvent->getChannel());
	
	switch (sendingStatus)
	{
	case sendingDone:{
			client->clearResponseAndRequest();
		}
		break;
	case clientClose: {
		curEvent->offboardQueue();
		client->clientKill();
		break;
	}
	case sending:
		break;
	default:
		break;
	}
}
void WriteEventToClientHandler::_handleRemain(ft::shared_ptr<Client> client, WriteEventToClient *curEvent){
	ft::shared_ptr<HttpRequest> curRequest = client->getRequest();
	ft::shared_ptr<VirtualServerManager> vsm = curEvent->getVirtualServerManger();
	PatternType patternType = client->getPatternType(vsm);
	PatternProcessor patternProcessor(vsm, patternType, client);
	ft::shared_ptr<Channel> channel = curEvent->getChannel();
		
	try {
		if (patternProcessor.querryCanSending() == SUCCESS)
			_partialSending(client->getResponse(), client, curEvent);
		// else {
		// 	EventQueue & eventQueue = EventQueue::getInstance();
		// 	EV_SET(
		// 	eventQueue.getEventSetElementPtr(),
		// 	channel->getFd(),
		// 	EVFILT_WRITE,
		// 	EV_DISABLE, 
		// 	0, 0, 
		// 	NULL);
		// 	if (kevent(eventQueue.getEventQueueFd(), eventQueue.getEventSet(), 1, NULL, 0, NULL) == -1) {
		// 		exit(1);
		// 		throw (KqueueError());
		// 	}
		// }
	}
	catch (HttpException &e) {
		_hanldeErrorPage(client, curEvent, e.getStatusCode());
	}
	catch (std::exception &e) {
		_hanldeErrorPage(client, curEvent, INTERNAL_SERVER_ERROR);
	}
}

void WriteEventToClientHandler::_handleEnd(WriteEventToClient *curEvent){
	curEvent->offboardQueue();
}

void WriteEventToClientHandler::_handleWait(void){
	// curEvent->offboardQueue();
	//do nothing
}

e_handle_status WriteEventToClientHandler::_queryHandleStatus(ft::shared_ptr<Client> client){
	if (client->isResponseEmpty() == false) {
		return (e_handle_remain);
	}
	if (client->isClientDie()) {
		if (client->isRequestEmpty() == false)
			return (e_handle_new);
		else
			return (e_handle_end);
	}
	else {
		if (client->isRequestEmpty() == false)
			return (e_handle_new);
		else
			return (e_handle_end);
	}
}

void WriteEventToClientHandler::_hanldeErrorPage(ft::shared_ptr<Client> client, WriteEventToClient *curEvent, HttpStatusCode statusCode){
	ft::shared_ptr<ErrorPageBuilder> errorPageBuilder(new ErrorPageBuilder(client, statusCode));

	errorPageBuilder->buildResponseHeader(client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey()));
	_partialSending(client->getResponse(), client, curEvent);
}

void WriteEventToClientHandler::_handleNew(ft::shared_ptr<Client> client, WriteEventToClient *curEvent){
	if (client->getRequest()->isError()){
		client->allocateResponse();
		ft::shared_ptr<HttpRequest> curRequest = client->getRequest();
		_hanldeErrorPage(client, curEvent, curRequest->getErrorStatusCode());
		std::string error = "Request Entity Too Large";
		Logger::getInstance().error(error);
		return ;
	}
	// client->getRequest()->_printBody();
	try {
		client->allocateResponse();
		ft::shared_ptr<HttpRequest> curRequest = client->getRequest();
		ft::shared_ptr<VirtualServerManager> vsm = curEvent->getVirtualServerManger();
		PatternType patternType = client->getPatternType(vsm);
		PatternProcessor patternProcessor(vsm, patternType, client);

		if (patternType == CGI_READ)
			patternProcessor.injectChannel(curEvent->getChannel());
		if (patternProcessor.process() == SUCCESS)
			_partialSending(client->getResponse(), client, curEvent);
	}
	catch (HttpException &e) {
		Logger::getInstance().error(e.what());
		_hanldeErrorPage(client, curEvent, e.getStatusCode());
	}
	catch (std::exception &e) {
		Logger::getInstance().error(e.what());
		_hanldeErrorPage(client, curEvent, INTERNAL_SERVER_ERROR);
	}
}

void WriteEventToClientHandler::handleEvent(Event &event){

	WriteEventToClient *curEvent = static_cast<WriteEventToClient *>(&event);
	ft::shared_ptr<Client> client = curEvent->getClient();
	e_handle_status handleStatus = _queryHandleStatus(client);

	switch (handleStatus)
	{
	case e_handle_remain:
		_handleRemain(client, curEvent);
		break;
	case e_handle_new:
		_handleNew(client, curEvent);
		break;
	case e_handle_wait:
		_handleWait();
		break;
	case e_handle_end:
		_handleEnd(curEvent);
		break;
	}
}