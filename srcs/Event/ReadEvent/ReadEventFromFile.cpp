#include <Event/ReadEvent/ReadEventFromFile.hpp>
#include <FileManager/FileManager/FileManager.hpp>
ReadEventFromFile::ReadEventFromFile(
    ft::shared_ptr<IoReadAndWriteBuffer> buffer,
    const std::string &path, std::string mode = "w") :
    ReadEvent(new ReadEventFromFileHandler(buffer,
        FileManager::getFileSize(path))),
    SingleStreamable(new FileStream(path, mode)) {}

ReadEventFromFile::~ReadEventFromFile(void) {}

void ReadEventFromFile::_syncWithFileTable(ft::shared_ptr<SyncroFileDataAndReader> syncroFileDataAndReader) {
    this->_syncroFileDataAndReader = syncroFileDataAndReader;
}
void ReadEventFromFile::callEventHandler(void) {
    std::cerr << "ReadEventFromFile::callEventHandler()" << std::endl;

    this->_event_handler->handleEvent(*this);
}
void ReadEventFromFile::onboardQueue(void) {
    Event *event = this;

    std::cerr << "ReadEventFromFile::onboardQueue()" << std::endl;
    try {
        this->getChannel()->setNonBlocking();
        this->_onboardRead(event, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        throw ;
    }
}

void ReadEventFromFile::offboardQueue(void) {
    std::cerr << "ReadEventFromFile::offboardQueue()" << std::endl;
    // exit(1);
    try {
        this->_offboardRead(this, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        throw ;
    }

}