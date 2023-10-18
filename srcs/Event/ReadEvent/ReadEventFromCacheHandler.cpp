#include <Event/ReadEvent/ReadEventFromCacheHandler.hpp>
#include <Event/ReadEvent/ReadEventFromCache.hpp>

ReadEventFromCacheHandler::ReadEventFromCacheHandler(std::vector<char> &content) : _content(content), _offset(0) {}
ReadEventFromCacheHandler::~ReadEventFromCacheHandler(void) {}

void ReadEventFromCacheHandler::handleEvent(Event &event) {
    ReadEventFromCache *readEventFromCache = static_cast<ReadEventFromCache *>(&event);
    
    size_t n;
    n = read(readEventFromCache->getFd(), _content.data() + _offset, _content.size() - _offset);
    if (n < 0) return ;
    _offset += n;
    if (_offset == _content.size()) {
        readEventFromCache->offboardQueue();
    }
}