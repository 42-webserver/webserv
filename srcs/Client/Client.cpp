#include <Client/Client.hpp>

Client::Client() : _eventQueueState(None) {}
Client::Client(e_client_event_queue_state eventQueueState) : _eventQueueState(eventQueueState) {}
Client::~Client() {
    ClientIdManager &idManager = ClientIdManager::getInstance(ClientIdManager::AcessKey());
    idManager.releaseId(this->_id);
}
void Client::build(e_client_role role){
    ClientIdManager &idManager = ClientIdManager::getInstance(ClientIdManager::AcessKey());
    this->_id = idManager.allocateId(role);
}
void Client::addRequest(ft::shared_ptr<HttpRequest> request){
    this->requests.push(request);
}

bool Client::isRequestEmpty(void){
    return this->requests.empty();
}

bool Client::isResponseEmpty(void){
    return !response.has_value();
}

bool Client::isQueueMax(void){
    return this->requests.size() >= MAX_QUEUE_SIZE;
}

e_client_event_queue_state Client::queryClientEventQueueState(void){
    return this->_eventQueueState;
}

void Client::addClientEventQueueState(e_client_event_queue_state state){
    this->_eventQueueState = (e_client_event_queue_state)(this->_eventQueueState | state);
}

void Client::removeClientEventQueueState(e_client_event_queue_state state){
    this->_eventQueueState = (e_client_event_queue_state)(this->_eventQueueState & ~state);
}

PatternType Client::getPatternType(ft::shared_ptr<VirtualServerManager> vsm){
    ft::shared_ptr<HttpRequest> request = this->requests.front();
    vsm->getDefaultVirtualServer();
    std::string method = request->getMethod();
    if (RouterUtils::isRedirection(vsm, request))
        return REDIRECTION;
    if (RouterUtils::isCgiRequest(vsm, request))
        return CGI_READ;
    if (method.compare(HTTP_METHOD::GET) == 0)
        return FILE_READ;
    else if (method.compare(HTTP_METHOD::PUT) == 0 ||
        method.compare(HTTP_METHOD::POST) == 0)
        return FILE_WRITE;
    else
        return DELETE;
}

