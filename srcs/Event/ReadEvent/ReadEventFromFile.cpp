#include <Event/ReadEvent/ReadEventFromFile.hpp>

ReadEventFromFile::ReadEventFromFile(
    IoReadAndWriteBuffer &buffer,
    const std::string &path, std::string mode = "w+", e_syncro_state *state = NULL) :
    ReadEvent(new ReadEventFromFileHandler(buffer)),
    SingleStreamable(new FileStream(path, mode)), _state(state) {}
ReadEventFromFile::~ReadEventFromFile(void) {
    this->_notifyFinish();
}
void ReadEventFromFile::_notifyFinish(void) {
    if (this->_state != NULL) {
        *this->_state = SyncroFinish;
    }
}
void ReadEventFromFile::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}
void ReadEventFromFile::onboardQueue(void) {
    Event *event = this;

    try {
        this->getChannel()->setNonBlocking();
        this->_onboardRead(event, this->getFd());
    }
    catch (KqueueError &e) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromFile", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromFile", "Fail to onboard Read Event");
        throw ;
    }
}
void ReadEventFromFile::offboardQueue(void) {

    try {
        this->_offboardRead(this, this->getFd());
    }
    catch (const std::exception &e) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromFile", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromFile", "Fail to offboard Read Event");
        throw ;
    }

}