#include <Client/Client.hpp>

Client::Client() {
    _queueState[InReading] = false;
    _queueState[InWriting] = false;
}
Client::~Client() {
    ClientIdManager &idManager = ClientIdManager::getInstance();
    idManager.releaseId(this->_id);
}
void Client::build(){
    ClientIdManager &idManager = ClientIdManager::getInstance();
    this->_id = idManager.allocateId();
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

bool Client::isInEventQueue(e_client_queue_state state){
    return this->_queueState[state];
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

