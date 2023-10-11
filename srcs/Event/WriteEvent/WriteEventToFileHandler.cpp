#include <Event/WriteEvent/WriteEventToFileHandler.hpp>
#include <Event/WriteEvent/WriteEventToFile.hpp>

WriteEventToFileHandler::WriteEventToFileHandler(ft::shared_ptr<e_syncro_state> state,
    IoReadAndWriteBuffer &buffer) :
    _state(state), _buffer(buffer) {}
WriteEventToFileHandler::~WriteEventToFileHandler(void) {}
void WriteEventToFileHandler::_notifyFinish() {
    *this->_state = SyncroFinish;
}
void WriteEventToFileHandler::handleEvent(Event &event) {
    WriteEventToFile *writeEventToFile = static_cast<WriteEventToFile *>(&event);
    size_t n;
    try {
     n = _buffer.ioWrite(writeEventToFile->getFd());
    }
    catch (const DisconnectionException &e) {
        throw ;
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToFileHandler", "Fail to write to file");
        throw ;
    }
    if (n == 0) {
        this->_notifyFinish();
        writeEventToFile->offboardQueue();
    }
}