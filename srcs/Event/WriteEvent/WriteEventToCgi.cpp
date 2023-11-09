#include <Event/WriteEvent/WriteEventToCgi.hpp>

WriteEventToCgi::WriteEventToCgi(ft::shared_ptr<Channel> channel,
ft::shared_ptr<Client> client): WriteEvent(new WriteEventToCgiHandler()), 
SingleStreamable(channel), _client(client) {
}

WriteEventToCgi::~WriteEventToCgi(void) {}

void	WriteEventToCgi::callEventHandler(void) {

    this->_event_handler->handleEvent(*this);
}

void	WriteEventToCgi::onboardQueue(void){
    Event *event = this;


    try {
        this->getChannel().get()->setNonBlocking();
        this->_onboardWrite(event, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        Logger::getInstance().error("Fail to onboard Write Event");
        throw ;
    }
}

void	WriteEventToCgi::offboardQueue(void){

    try {
        this->_offboardWrite(this, this->getFd());
    }
    catch (...) {
        Logger::getInstance().error("Fail to offboard Write Event");
        throw ;
    }
}

ft::shared_ptr<Client> WriteEventToCgi::getClient(void)  {
    return (this->_client);
}