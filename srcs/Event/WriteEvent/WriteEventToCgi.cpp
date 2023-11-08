#include <Event/WriteEvent/WriteEventToCgi.hpp>

WriteEventToCgi::WriteEventToCgi(ft::shared_ptr<Channel> channel,
ft::shared_ptr<Client> client): WriteEvent(new WriteEventToCgiHandler()), 
SingleStreamable(channel), _client(client) {
}

WriteEventToCgi::~WriteEventToCgi(void) {}

void	WriteEventToCgi::callEventHandler(void) {
    std::cerr << "WriteEventToCgi::callEventHandler" << std::endl;

    this->_event_handler->handleEvent(*this);
}

void	WriteEventToCgi::onboardQueue(void){
    Event *event = this;

    std::cerr << "WriteEventToCgi::onboardQueue" << std::endl;

    try {
        this->getChannel().get()->setNonBlocking();
        this->_onboardWrite(event, this->getFd());
    }
    catch (KqueueError &e) {
        std::cerr << "WriteEventToCgi::onboardQueue KqueueError" << std::endl;
        throw (KqueueError());
    }
    catch (std::exception &e) {
        std::cerr << "WriteEventToCgi::onboardQueue ..." << std::endl;
        Logger::getInstance().error("Fail to onboard Write Event");
        throw ;
    }
}

void	WriteEventToCgi::offboardQueue(void){
    std::cerr << "WriteEventToCgi::offboardQueue" << std::endl;

    try {
        this->_offboardWrite(this, this->getFd());
    }
    catch (...) {
        std::cerr << "WriteEventToCgi::offboardQueue ..." << std::endl;
        Logger::getInstance().error("Fail to offboard Write Event");
        throw ;
    }
    std::cerr << "WriteEventToCgi::offboardQueue end" << std::endl;
}

ft::shared_ptr<Client> WriteEventToCgi::getClient(void)  {
    return (this->_client);
}