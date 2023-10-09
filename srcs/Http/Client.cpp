#include <Http/Client.hpp>

Client::Client(){
}

void Client::addRequest(ft::shared_ptr<HttpRequest> request){
    this->requests.push(request);
}

bool Client::isRequestEmpty(void){
    return this->requests.empty();
}

int Client::getQueueSize(void){
    return this->requests.size();
}

EventType Client::getEventType(ft::shared_ptr<VirtualServerManager> vsm){
    ft::shared_ptr<HttpRequest> request = this->requests.front();
    vsm->getDefaultVirtualServer();
    std::string method = request->getMethod();
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

