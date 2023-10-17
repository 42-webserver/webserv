#include <Event/WriteEvent/WriteEventToCache.hpp>

WriteEventToCache::WriteEventToCache(
    std::vector<char> &content,
    const std::string &path, std::string mode) : 
    WriteEvent(new WriteEventToCacheHandler(content)),
    SingleStreamable(new FileStream(path, mode)){}

WriteEventToCache::~WriteEventToCache(void){}

void WriteEventToCache::_syncWithCache(ft::shared_ptr<SyncroWriteWithCache> syncroWriteWithCache) {
    this->_syncroWriteWithCache = syncroWriteWithCache;
}

void WriteEventToCache::callEventHandler(void) {
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
        Logger::getInstance().error("{} {}", 2, "WriteEventToCache", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToCache", "Fail to onboard Write Event");
        throw ;
    }
}
void WriteEventToCache::offboardQueue(void) {
    try {
        this->_offboardWrite(this, this->getFd());
    }
    catch (const std::exception &e) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToCache", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "WriteEventToCache", "Fail to offboard Write Event");
        throw ;
    }
}