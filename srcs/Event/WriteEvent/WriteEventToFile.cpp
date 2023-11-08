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
    std::cerr << "WriteEventToFile::callEventHandler()" << std::endl;
    this->_event_handler->handleEvent(*this);
}
void WriteEventToFile::onboardQueue(void) {

    std::cerr << "WriteEventToFile::onboardQueue()" << std::endl;
    try {
        this->getChannel()->setNonBlocking();
        this->_onboardWrite(this, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        throw ;
    }
}
void WriteEventToFile::offboardQueue(void) {

    std::cerr << "WriteEventToFile::offboardQueue()" << std::endl;
    try {
        this->_offboardWrite(this, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        throw ;
    }

}