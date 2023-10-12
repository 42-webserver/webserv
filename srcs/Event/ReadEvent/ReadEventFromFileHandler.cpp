#include <Event/ReadEvent/ReadEventFromFileHandler.hpp>
#include <Event/ReadEvent/ReadEventFromFile.hpp>
ReadEventFromFileHandler::ReadEventFromFileHandler(IoReadAndWriteBuffer &buffer) :_buffer(buffer) {}
ReadEventFromFileHandler::~ReadEventFromFileHandler(void) {}

void ReadEventFromFileHandler::handleEvent(Event &event) {
    ReadEventFromFile *readEventFromFile = static_cast<ReadEventFromFile *>(&event);
   size_t n = _buffer.ioRead(readEventFromFile->getFd());

   if (n == 0) {
       readEventFromFile->offboardQueue();
   }
}
