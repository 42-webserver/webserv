#include <Client/Client.hpp>
#include <Http/Exception/MethodNotAllowedException.hpp>
Client::Client(
e_client_event_queue_state eventQueueState) : _eventQueueState(eventQueueState ){
}

Client::~Client() {
}

void Client::clientKill(){
    _eventQueueState = None;
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

bool Client::isClientDie(void){
    return this->_eventQueueState == None;
}

e_client_event_queue_state Client::queryClientEventQueueState(void){
    return this->_eventQueueState;
}

ft::shared_ptr<HttpRequest> Client::getRequest(void){
    return this->requests.front();
}

void Client::processCurrentRequestDone(void){
    this->requests.pop();
}

bool Client::isEventQueueStateTurnOn(e_client_event_queue_state state){
    return (this->_eventQueueState & state) == state;
}
void Client::addClientEventQueueState(e_client_event_queue_state state){
    this->_eventQueueState = (e_client_event_queue_state)(this->_eventQueueState | state);
}

void Client::removeClientEventQueueState(e_client_event_queue_state state){
    this->_eventQueueState = (e_client_event_queue_state)(this->_eventQueueState & ~state);
}

ft::shared_ptr<HttpResponse> Client::getResponse(void){
    return this->response.value();
}

void Client::allocateResponse(void){
    this->response = ft::shared_ptr<HttpResponse>(new HttpResponse());
}

void Client::setResponse(ft::shared_ptr<HttpResponse> response){
    this->response = response;
}

void Client::clearResponseAndRequest(void){
    this->response.reset();
    this->requests.pop();
}
PatternType Client::getPatternType(const ft::shared_ptr<VirtualServerManager>& vsm) const {
    ft::shared_ptr<HttpRequest> request = this->requests.front();
    vsm->getDefaultVirtualServer();
    std::string method = request->getMethod();

    if (!RouterUtils::isMethodAllowed(vsm, request))
        throw MethodNotAllowedException();
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

