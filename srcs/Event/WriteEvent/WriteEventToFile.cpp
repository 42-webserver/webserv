#include <Event/WriteEvent/WriteEventToFile.hpp>

WriteEventToFile::WriteEventToFile(
    IoReadAndWriteBuffer &buffer,
    const std::string &path, std::string mode = "w+", e_syncro_state *state = NULL) :
    WriteEvent(new WriteEventToFileHandler(buffer)),
    SingleStreamable(new FileStream(path, mode)), _state(state) {}
WriteEventToFile::~WriteEventToFile(void) {
    this->_notifyFinish();
}
void WriteEventToFile::_notifyFinish(void) {
    if (this->_state != NULL) {
        *this->_state = SyncroFinish;
    }
}
void WriteEventToFile::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}
void WriteEventToFile::onboardQueue(void) {

    try {
        this->getChannel()->setNonBlocking();
        this->_onboardWrite(this, this->getFd());
    }
    catch (KqueueError &e) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToFile", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToFile", "Fail to onboard Write Event");
        throw ;
    }
}
void WriteEventToFile::offboardQueue(void) {

    try {
        this->_offboardWrite(this, this->getFd());
    }
    catch (const std::exception &e) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToFile", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToFile", "Fail to offboard Write Event");
        throw ;
    }

}