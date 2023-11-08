#include <Event/WriteEvent/WriteEventToCache.hpp>

WriteEventToCache::WriteEventToCache(
    ft::shared_ptr<IoReadAndWriteBuffer> buffer,
    const std::string &path, std::string mode) : 
    WriteEvent(new WriteEventToCacheHandler(buffer)),
    SingleStreamable(new FileStream(path, mode)){}

WriteEventToCache::~WriteEventToCache(void){}

void WriteEventToCache::_syncWithCache(ft::shared_ptr<SyncroWriteWithCache> syncroWriteWithCache) {
    this->_syncroWriteWithCache = syncroWriteWithCache;
}

void WriteEventToCache::callEventHandler(void) {
    std::cerr << "WriteEventToCache::callEventHandler()" << std::endl;
    this->_event_handler->handleEvent(*this);
}

bool WriteEventToCache::isFinalWriter(void) {
    return (this->_syncroWriteWithCache->isFinalWriter());
}

void WriteEventToCache::onboardQueue(void) {

    try {
        this->getChannel()->setNonBlocking();
        this->_onboardWrite(this, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        throw ;
    }
}
void WriteEventToCache::offboardQueue(void) {
    try {
        this->_offboardWrite(this, this->getFd());
    }
    catch (const std::exception &e) {
        throw (KqueueError());
    }
    catch (...) {
        throw ;
    }
}