#include <FileManager/Cache/cache.hpp>
#include <fstream>
Cache *Cache::_instance = NULL;
Cache::Cache(void) {}
Cache::~Cache(void) {}


void Cache::deleteInstance(void) {
	if (Cache::_instance != NULL) {
		delete Cache::_instance;
		Cache::_instance = NULL;
	}
}
Cache	&Cache::getInstance(void) {
	if (Cache::_instance == NULL) {
		Cache::_instance = new Cache();
	}
	return (*Cache::_instance);
}


bool Cache::hit(const std::string &uri) {
	//Todo : fix this!
	return (this->_cache.get(uri).size() != 0);
}
// size_t	Cache::copyFromCacheTo(IoReadAndWriteBuffer &buffer, const std::string &uri) {
	
// 	size_t size = buffer.append(_cache.getIter(uri), this->_cache.getCacheContentSize(uri));
// 	return (size);
// }

void    Cache::getCacheContent(const std::string &uri, std::vector<char> &buffer) {
	const std::vector<char> &tmp = this->_cache.get(uri);
	buffer.insert(buffer.end(), tmp.begin(), tmp.end());
}

size_t	Cache::getCacheContentSize(const std::string &uri)  {
	return (_cache.getCacheContentSize(uri));
}

void    Cache::putCacheContent(const std::string &uri) {
	this->_cache.put(uri);
}

void	Cache::putCacheContent(const std::string &uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer) {
	this->_cache.put(uri, buffer);
}

void	Cache::deleteCacheContent(const std::string &uri) {
	this->_cache.deleteContent(uri);
}
