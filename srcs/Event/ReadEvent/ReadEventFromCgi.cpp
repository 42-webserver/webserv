#include <Event/ReadEvent/ReadEventFromCgi.hpp>

ReadEventFromCgi::ReadEventFromCgi(ft::shared_ptr<Channel> readPipe,
    ft::shared_ptr<Client> client):
    ReadEvent(new ReadEventFromCgiHandler()),
    SingleStreamable(readPipe),
    _client(client) {}
ReadEventFromCgi::~ReadEventFromCgi(){}

void ReadEventFromCgi::callEventHandler(void){
    this->_event_handler->handleEvent(*this);
}

void ReadEventFromCgi::onboardQueue(void){
    int fd = this->getFd();
    Event *event = this;

    try {
        this->getChannel()->setNonBlocking();
        this->_onboardRead(event, fd);
    } catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (...) {
        throw ;
    }
}

void ReadEventFromCgi::offboardQueue(void){
    int fd = this->getFd();
    Event *event = this;

    try {
        this->_offboardRead(event, fd);
    } catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (...) {
        throw ;
    }
}

ft::shared_ptr<Client> ReadEventFromCgi::getClient(void) const{
    return this->_client;
}