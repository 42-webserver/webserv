#include <Event/ReadEvent/ReadEventFromFileHandler.hpp>
#include <Event/ReadEvent/ReadEventFromFile.hpp>
ReadEventFromFileHandler::ReadEventFromFileHandler(ft::shared_ptr<e_syncro_state> state,
    IoReadAndWriteBuffer &buffer) :
    _state(state), _buffer(buffer) {}
ReadEventFromFileHandler::~ReadEventFromFileHandler(void) {}
void ReadEventFromFileHandler::_notifyFinish() {
    *this->_state = SyncroFinish;
}
void ReadEventFromFileHandler::handleEvent(Event &event) {
    ReadEventFromFile *readEventFromFile = static_cast<ReadEventFromFile *>(&event);
   size_t n = _buffer.ioRead(readEventFromFile->getFd());

   if (n == 0) {
       this->_notifyFinish();
       readEventFromFile->offboardQueue();
   }
}
