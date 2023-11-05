#include <Event/WriteEvent/WriteEventToFile.hpp>

WriteEventToFile::WriteEventToFile(
    ft::shared_ptr<IoReadAndWriteBuffer> buffer,
    const std::string &path, std::string mode = "w+") :
    WriteEvent(new WriteEventToFileHandler(buffer)),
    SingleStreamable(new FileStream(path, mode)){}

WriteEventToFile::~WriteEventToFile(void) {
}

void WriteEventToFile::_syncWithFileTable(ft::shared_ptr<SyncroFileDataAndWriter> syncroFileDataAndWriter) {
    this->_syncroFileDataAndWriter = syncroFileDataAndWriter;
}

void WriteEventToFile::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}
void WriteEventToFile::onboardQueue(void) {

    std::cerr << "WriteEventToFile::onboardQueue()" << std::endl;
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

    std::cerr << "WriteEventToFile::offboardQueue()" << std::endl;
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