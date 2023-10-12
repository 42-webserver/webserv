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
	return (this->_cache.get(uri).size() != 0);
}
size_t	Cache::copyFromCacheTo(IoReadAndWriteBuffer &buffer, const std::string &uri) {
	
	size_t size = buffer.append(_cache.getIter(uri), this->_cache.getCacheContentSize(uri));
	return (size);
}
size_t	Cache::getCacheContentSize(const std::string &uri)  {
	return (_cache.getCacheContentSize(uri));
}

