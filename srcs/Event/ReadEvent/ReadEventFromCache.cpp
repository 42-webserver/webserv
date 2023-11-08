#include <Event/ReadEvent/ReadEventFromCache.hpp>
#include <FileManager/FileManager/FileManager.hpp>
ReadEventFromCache::ReadEventFromCache(std::vector<char> &content, const std::string &path, std::string mode
= "w") : 
    ReadEvent(new ReadEventFromCacheHandler(content,
    FileManager::getFileSize(path))),
    SingleStreamable(new FileStream(path, mode)) {}

ReadEventFromCache::~ReadEventFromCache(void) {}

void ReadEventFromCache::_syncWithCache(ft::shared_ptr<SyncroReadWithCache> syncroReadWithCache){
    this->_syncroReadWithCache = syncroReadWithCache;
}

void ReadEventFromCache::callEventHandler(void) {
    std::cerr << "ReadEventFromCache::callEventHandler" << std::endl;
    this->_event_handler->handleEvent(*this);
}

void ReadEventFromCache::onboardQueue(void) {
    std::cerr << "ReadEventFromCache::onboardQueue" << std::endl;
    try {
        this->getChannel()->setNonBlocking();
        this->_onboardRead(this, this->getFd());
    }
    catch (KqueueError &e) {
        Logger::getInstance().error("kqueue error ");
        throw (KqueueError());
    }
    catch (std::exception &e) {
        Logger::getInstance().error(e.what());
        throw ;
    }

}

void ReadEventFromCache::offboardQueue(void) {
    std::cerr << "ReadEventFromCache::offboardQueue" << std::endl;
    try {
        this->_offboardRead(this, this->getFd());
    }
    catch (KqueueError &e) {
        throw (KqueueError());
    }
    catch (std::exception &e) {
        throw ;
    }
}