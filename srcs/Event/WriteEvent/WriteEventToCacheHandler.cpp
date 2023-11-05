#include <Event/WriteEvent/WriteEventToCacheHandler.hpp>
#include <Event/WriteEvent/WriteEventToCache.hpp>

WriteEventToCacheHandler::WriteEventToCacheHandler(
ft::shared_ptr<IoReadAndWriteBuffer> buffer) : _buffer(buffer){}
WriteEventToCacheHandler::~WriteEventToCacheHandler(void) {}

void WriteEventToCacheHandler::handleEvent(Event &event) {
    WriteEventToCache *writeEventToCache = static_cast<WriteEventToCache *>(&event);
    
    if (!writeEventToCache->isFinalWriter()) {
        writeEventToCache->offboardQueue();
        return ;
    }
    try {
        _buffer->ioWrite(writeEventToCache->getFd());
    }
    catch (const DisconnectionException &e) {
        throw ;
    }
    catch (...) {
        throw ;
    }
    if (_buffer->size() == 0) {
        writeEventToCache->offboardQueue();
    }
}