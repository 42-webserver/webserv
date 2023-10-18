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
    size_t n;
    try {
     n = _buffer->ioWrite(writeEventToCache->getFd());
    }
    catch (const DisconnectionException &e) {
        throw ;
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToCacheHandler", "Fail to write to cache");
        throw ;
    }
    if (n == 0) {
        writeEventToCache->offboardQueue();
    }
}