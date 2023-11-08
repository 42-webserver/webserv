#include <Event/ReadEvent/ReadEventFromCgi.hpp>

ReadEventFromCgi::ReadEventFromCgi(ft::shared_ptr<Channel> readPipe,
    ft::shared_ptr<Client> client):
    ReadEvent(new ReadEventFromCgiHandler()),
    SingleStreamable(readPipe),
    _client(client) {}
ReadEventFromCgi::~ReadEventFromCgi(){}

void ReadEventFromCgi::callEventHandler(void){
    std::cerr << "ReadEventFromCgi::callEventHandler" << std::endl;
    this->_event_handler->handleEvent(*this);
}

void ReadEventFromCgi::onboardQueue(void){
    std::cerr << "ReadEventFromCgi::onboardQueue" << std::endl;
    std::cerr << "ReadEventFromCgi::onboardQueue getFd" << std::endl;
    int fd = this->getFd();
    Event *event = this;

    try {
        std::cerr << "ReadEventFromCgi::onboardQueue setNonBlocking" << std::endl;
        this->getChannel()->setNonBlocking();
        this->_onboardRead(event, fd);
        std::cerr << "ReadEventFromCgi::onboardQueue _onboardRead end" << std::endl;
    } catch (KqueueError &e) {
        std::cerr << "ReadEventFromCgi::onboardQueue KqueueError" << e.what() << std::endl;
        exit(1);
        throw (KqueueError());
    }
    catch (std::exception &e) {
        std::cerr << "ReadEventFromCgi::onboardQueue ..." << e.what() << std::endl;
        exit(1);
        throw ;
    }
}

void ReadEventFromCgi::offboardQueue(void){
    int fd = this->getFd();
    Event *event = this;
    std::cerr << "ReadEventFromCgi::offboardQueue" << std::endl;

    try {
        this->_offboardRead(event, fd);
    } catch (KqueueError &e) {
        std::cerr << e.what() << '\n';
        throw (KqueueError());
    }
    catch (...) {
        throw ;
    }
}

ft::shared_ptr<Client> ReadEventFromCgi::getClient(void) const{
    return this->_client;
}