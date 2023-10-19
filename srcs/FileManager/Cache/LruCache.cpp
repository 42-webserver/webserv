#include <FileManager/Cache/LruCache.hpp>
#include <FileManager/FileManager/FileManager.hpp>
#include <Event/WriteEvent/WriteEventToCache.hpp>
#include <Event/ReadEvent/ReadEventFromCache.hpp>
SyncroWriteWithCache::SyncroWriteWithCache(LruCacheNode &lruCache): lruCache(lruCache), _thisWriterNum(lruCache.getFinalWriterNum() + 1) {
	this->lruCache.setWriting();
}

SyncroWriteWithCache::~SyncroWriteWithCache(void) {
	if (this->lruCache.isFinalWriter(this->_thisWriterNum)) {
		this->lruCache.updateFinsish();
	}
}

bool SyncroWriteWithCache::isFinalWriter(void) {
	return (this->lruCache.isFinalWriter(this->_thisWriterNum));
}

SyncroReadWithCache::SyncroReadWithCache(LruCacheNode &lruCache): lruCache(lruCache) {
	this->lruCache.setReading();
}

SyncroReadWithCache::~SyncroReadWithCache(void) {
	this->lruCache.updateFinsish();
}

LruCacheNode::LruCacheNode(void): _content(), _status(e_done), _finalWriterNum(0) {
	_content.reserve(LruCache::_BlockSize);
}

LruCacheNode::~LruCacheNode(void) {}

LruCacheNode::LruCacheNode(ft::shared_ptr<IoReadAndWriteBuffer> buffer): _content(), _status(e_done), _finalWriterNum(0) {
	_content.reserve(LruCache::_BlockSize);
	buffer->copyHeadTo(this->_content);
}


bool LruCacheNode::isUpdatedContent(void) {
	return (this->_status == e_done);
}

e_cache_node_status LruCacheNode::getStatus(void) {
	return (this->_status);
}
void LruCacheNode::setWriting(void) {
	this->_status = e_writing;
	_finalWriterNum++;
}

void LruCacheNode::setReading(void) {
	this->_status = e_reading;
}

void LruCacheNode::updateFinsish(void) {
	this->_status = e_done;
	this->_finalWriterNum = 0;
}

bool LruCacheNode::getFinalWriterNum(void) {
	return (this->_finalWriterNum);
}

bool LruCacheNode::isFinalWriter(int writerNum) {
	return (this->_finalWriterNum == writerNum);
}

std::vector<char>	&LruCacheNode::getContent(void) {
	return (this->_content);
}

size_t				LruCacheNode::getContentSize(void) {
	return (this->_content.size());
}

ft::shared_ptr<SyncroReadWithCache> LruCacheNode::buildSyncroReadWithCache(void) {
	return (ft::shared_ptr<SyncroReadWithCache>(new SyncroReadWithCache(*this)));
}

ft::shared_ptr<SyncroWriteWithCache> LruCacheNode::buildSyncroWriteWithCache(void) {
	return (ft::shared_ptr<SyncroWriteWithCache>(new SyncroWriteWithCache(*this)));
}

LruCache::LruCache(void)  {}
LruCache::~LruCache(void) {}


void LruCache::_readToCache(const std::string &uri) {
	struct stat fileStat;
	e_file_info fileInfo = FileManager::getFileInfo(uri, fileStat);
	if (fileInfo == NotExistFile) {
		throw FileNotExistException(uri);
	}
	if (fileInfo == ExistDirectory) {
		throw FileIsDirectoryException(uri);
	}
	EventFactory &eventFactory = EventFactory::getInstance();
	LruCacheNode &lruCacheNode = (this->_cache[uri])->second;
	ft::shared_ptr<SyncroReadWithCache> syncroReadWithCache = lruCacheNode.buildSyncroReadWithCache();
	EventDto eventDto(&(lruCacheNode.getContent()), uri, "r");
	ReadEventFromCache *readEventFromCache = static_cast<ReadEventFromCache *>
	(eventFactory.createEvent(ft::CACHE_READ_EVENT, eventDto));

	readEventFromCache->_syncWithCache(syncroReadWithCache);
	readEventFromCache->onboardQueue();
}

void LruCache::_writeToCache(const std::string &uri,
 ft::shared_ptr<IoReadAndWriteBuffer> buffer) {
	struct stat fileStat;

	e_file_info fileInfo = FileManager::getFileInfo(uri, fileStat);
	if (fileInfo == ExistDirectory) {
		throw FileIsDirectoryException(uri);
	}

	EventFactory &eventFactory = EventFactory::getInstance();
	LruCacheNode &lruCacheNode = (this->_cache[uri])->second;
	ft::shared_ptr<SyncroWriteWithCache> syncroWriteWithCache = lruCacheNode.buildSyncroWriteWithCache();
	EventDto eventDto(buffer, uri, "w");
	WriteEventToCache *writeEventToCache = static_cast<WriteEventToCache *>(eventFactory.createEvent
	(ft::CACHE_WRITE_EVENT, eventDto));

	writeEventToCache->_syncWithCache(syncroWriteWithCache);
	writeEventToCache->onboardQueue();
}
const std::vector<char>	&LruCache::get(const std::string &uri){
	
	if (hit(uri) == false) {
		return (this->empty);
	}

	this->_lru_list.splice(this->_lru_list.begin(), this->_lru_list, this->_cache[uri]);
	return (this->_cache[uri]->second.getContent());
}

bool LruCache::hit(const std::string &uri) {
	return (this->_cache.find(uri) != this->_cache.end())
	&& (this->_cache[uri]->second.getStatus() == e_done);
}
size_t				LruCache::getCacheContentSize(const std::string &uri){
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (0);
	}
	return (this->_cache[uri]->second.getContentSize());
}


//hit을 가정안하고 없다 가정하에, 클라이언트 버퍼를받아와서 그내용 넣음
void LruCache::put(const std::string& uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer) {
	
   if (this->_cache.size() >= this->_capacity) {
        lru_list_t::iterator last = this->_lru_list.end();
        last--;

        this->_cache.erase(last->first);  // 가장 오래된 항목의 키로 삭제
        this->_lru_list.pop_back();
    }

	this->_lru_list.push_front(std::make_pair(uri, 
	LruCacheNode(buffer)));
	_writeToCache(uri, buffer);
	this->_cache[uri] = this->_lru_list.begin();
}
void LruCache::put(const std::string& uri){
   if (this->_cache.size() >= this->_capacity) {
        lru_list_t::iterator last = this->_lru_list.end();
        last--;

        this->_cache.erase(last->first);  // 가장 오래된 항목의 키로 삭제
        this->_lru_list.pop_back();
    }

	this->_lru_list.push_front(std::make_pair(uri, 
	LruCacheNode()));
	_readToCache(uri);
	this->_cache[uri] = this->_lru_list.begin();	
}

//Todo :check
void LruCache::deleteContent(const std::string &uri) {
	if (this->_cache.find(uri) == this->_cache.end()) {
		return ;
	}
	this->_lru_list.erase(this->_cache[uri]);
	this->_cache.erase(uri);
	unlink(uri.c_str());
}
const char	*LruCache::FailToGetException::what(void) const throw() { return ("LruCache: Fail to get"); }