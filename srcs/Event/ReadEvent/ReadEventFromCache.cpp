#include <Event/ReadEvent/ReadEventFromCache.hpp>

ReadEventFromCache::ReadEventFromCache(std::vector<char> &content, const std::string &path, std::string mode
= "w") : 
    ReadEvent(new ReadEventFromCacheHandler(content)),
    SingleStreamable(new FileStream(path, mode)) {}

ReadEventFromCache::~ReadEventFromCache(void) {}

void ReadEventFromCache::_syncWithCache(ft::shared_ptr<SyncroReadWithCache> syncroReadWithCache){
    this->_syncroReadWithCache = syncroReadWithCache;
}

void ReadEventFromCache::callEventHandler(void) {
    this->_event_handler->handleEvent(*this);
}

void ReadEventFromCache::onboardQueue(void) {

    try {
        this->getChannel()->setNonBlocking();
        this->_onboardRead(this, this->getFd());
    }
    catch (KqueueError &e) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromCache", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromCache", "Fail to onboard Read Event");
        throw ;
    }

}

void ReadEventFromCache::offboardQueue(void) {

    try {
        this->_offboardRead(this, this->getFd());
    }
    catch (const std::exception &e) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromCache", e.what());
        throw (KqueueError());
    }
    catch (...) {
        Logger::getInstance().error("{} {}", 2, "ReadEventFromCache", "Fail to offboard Read Event");
        throw ;
    }

}