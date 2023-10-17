#include <FileManager/Cache/LruCache.hpp>

LruCache::LruCache(void)  {}
LruCache::~LruCache(void) {}

const std::vector<char>	&LruCache::get(const std::string &uri){
	
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (this->empty);
	}

	this->_lru_list.splice(this->_lru_list.begin(), this->_lru_list, this->_cache[uri]);
	return (this->_cache[uri]->second);
}

size_t				LruCache::getCacheContentSize(const std::string &uri){
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (0);
	}
	return (this->_cache[uri]->second.size());
}
std::vector<char>::iterator	LruCache::getIter(const std::string &uri){
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (this->empty.end());
	}
	this->_lru_list.splice(this->_lru_list.begin(), this->_lru_list, this->_cache[uri]);
	return (this->_cache[uri]->second.begin());
}
void				LruCache::put(std::string uri, std::vector<char> content) {
	if (this->_cache.size() >= this->_capacity) {
		lru_list_t::iterator	last = this->_lru_list.end();

		last--;
		this->_cache.erase(last->first);
		this->_lru_list.pop_back();
	}

	this->_lru_list.push_front(std::make_pair(uri, content));
	this->_cache[uri] = this->_lru_list.begin();
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