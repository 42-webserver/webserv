#include <FileManager/Cache/LruCache.hpp>
#include <FileManager/FileManager/FileManger.hpp>
#include <Event/WriteEvent/WriteEventToCache.hpp>
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
	//Todo::
	
}

void LruCache::_writeToCache(const std::string &uri){
	struct stat fileStat;
	e_file_info fileInfo = FileManager::getFileInfo(uri, fileStat);
	if (fileInfo == ExistDirectory) {
		throw FileIsDirectoryException(uri);
	}

	EventFactory &eventFactory = EventFactory::getInstance();
	LruCacheNode &lruCacheNode = *(this->_cache[uri]);
	ft::shared_ptr<SyncroWriteWithCache> syncroWriteWithCache = lruCacheNode.buildSyncroWriteWithCache();
	EventDto eventDto(&(lruCacheNode.getContent()), uri, "w");
	WriteEventToCache *writeEventToCache = static_cast<WriteEventToCache *>(eventFactory.createEvent
	(ft::CACHE_WRITE_EVENT, eventDto));

	writeEventToCache->_syncWithCache(syncroWriteWithCache);
	writeEventToCache->onboardQueue();
}
const std::vector<char>	&LruCache::get(const std::string &uri){
	
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (this->empty);
	}

	this->_lru_list.splice(this->_lru_list.begin(), this->_lru_list, this->_cache[uri]);
	return (this->_cache[uri]->getContent());
}

bool LruCache::hit(const std::string &uri) {
	return (this->_cache.find(uri) != this->_cache.end())
	&& (this->_cache[uri]->getStatus() == e_done);
}
size_t				LruCache::getCacheContentSize(const std::string &uri){
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (0);
	}
	return (this->_cache[uri]->getContentSize());
}
// std::vector<char>::iterator	LruCache::getIter(const std::string &uri){
// 	if (this->_cache.find(uri) == this->_cache.end()) {
// 		return (this->empty.end());
// 	}
// 	this->_lru_list.splice(this->_lru_list.begin(), this->_lru_list, this->_cache[uri]);
// 	return (this->_cache[uri]->second.begin());
// }
void				LruCache::put(std::string uri, std::vector<char> content) {
	//Todo :
	(void)uri;
	(void)content;
	// if (this->_cache.size() >= this->_capacity) {
	// 	lru_list_t::iterator	last = this->_lru_list.end();

	// 	last--;
	// 	this->_cache.erase(last->first);
	// 	this->_lru_list.pop_back();
	// }

	// this->_lru_list.push_front(std::make_pair(uri, content));
	// this->_cache[uri] = this->_lru_list.begin();
}
void LruCache::put(const std::string& uri) {
	//Todo :
	(void)uri;
}

//있는거면 덮어씌우게 uri로 찾았을떄 해야될듯 ?
void LruCache::put(const std::string& uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer) {
	//Todo :
	(void)uri;
	(void)buffer;
}

void LruCache::deleteContent(const std::string &uri) {
	//Todo :
	(void)uri;
}
const char	*LruCache::FailToGetException::what(void) const throw() { return ("LruCache: Fail to get"); }