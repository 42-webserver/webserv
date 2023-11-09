#include <Event/ReadEvent/ReadEventFromFileHandler.hpp>
#include <Event/ReadEvent/ReadEventFromFile.hpp>
ReadEventFromFileHandler::ReadEventFromFileHandler(ft::shared_ptr<IoReadAndWriteBuffer> buffer,
    ssize_t fileSize) : _buffer(buffer), _fileSize(fileSize), _offset(0) {}
ReadEventFromFileHandler::~ReadEventFromFileHandler(void) {}

void ReadEventFromFileHandler::handleEvent(Event &event) {
    ReadEventFromFile *readEventFromFile = static_cast<ReadEventFromFile *>(&event);
   size_t n = _buffer->ioRead(readEventFromFile->getFd());

    if (n < 0)
        throw std::runtime_error("ReadEventFromFileHandler::handleEvent: ioRead error");
    this->_offset += n;
    if (this->_offset == this->_fileSize) {
        readEventFromFile->offboardQueue();
    }
}
