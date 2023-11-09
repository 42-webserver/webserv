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
    this->_event_handler->handleEvent(*this);
}
void ReadEventFromFile::onboardQueue(void) {
    Event *event = this;

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