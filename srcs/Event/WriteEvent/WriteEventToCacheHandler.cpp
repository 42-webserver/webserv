#include <Event/WriteEvent/WriteEventToCacheHandler.hpp>
#include <Event/WriteEvent/WriteEventToCache.hpp>

WriteEventToCacheHandler::WriteEventToCacheHandler(
std::vector<char> &content) : _content(content) {}
WriteEventToCacheHandler::~WriteEventToCacheHandler(void) {}

void WriteEventToCacheHandler::handleEvent(Event &event) {
    WriteEventToCache *writeEventToCache = static_cast<WriteEventToCache *>(&event);
    
    if (!writeEventToCache->isFinalWriter()) {
        writeEventToCache->offboardQueue();
        return ;
    }
    size_t n;
    n = write(writeEventToCache->getFd(), _content.data() + _offset, _content.size() - _offset);
    if (n < 0) return ;
    _offset += n;
    if (_offset == _content.size()) {
        writeEventToCache->offboardQueue();
    }
}