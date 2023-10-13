#include <Event/ReadEvent/ReadEventFromFile.hpp>

ReadEventFromFile::ReadEventFromFile(
    IoReadAndWriteBuffer &buffer,
    const std::string &path, std::string mode = "w+") :
    ReadEvent(new ReadEventFromFileHandler(buffer)),
    SingleStreamable(new FileStream(path, mode)) {}

ReadEventFromFile::~ReadEventFromFile(void) {}

void ReadEventFromFile::_sync(e_file_content_syncro *origin,
    e_file_content_syncro targetState, bool *haveToUpdate) {
    _syncObj.Sync(origin, targetState, haveToUpdate);
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