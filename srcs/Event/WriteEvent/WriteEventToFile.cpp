#include <Event/WriteEvent/WriteEventToFile.hpp>

WriteEventToFile::WriteEventToFile(
    IoReadAndWriteBuffer &buffer,
    const std::string &path, std::string mode = "w+") :
    WriteEvent(new WriteEventToFileHandler(buffer)),
    SingleStreamable(new FileStream(path, mode)){}

WriteEventToFile::~WriteEventToFile(void) {
}

void WriteEventToFile::_sync(e_file_content_syncro *origin,
    e_file_content_syncro targetState, bool *haveToUpdate) {
    _syncObj.Sync(origin, targetState, haveToUpdate);
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