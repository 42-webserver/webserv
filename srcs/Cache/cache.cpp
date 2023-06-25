#include "../../incs/Cache/Cache.hpp"

//Refactoring : daegulee! cache reload update
LruCache::LruCache(void): _capacity(0) {}
LruCache::~LruCache(void) {}
LruCache::LruCache(int capacity): _capacity(capacity) {}

std::vector<char>	LruCache::get(std::string uri) {
	if (this->_cache.find(uri) == this->_cache.end()) {
		return (std::vector<char>());
	}

	this->_lru_list.splice(this->_lru_list.begin(), this->_lru_list, this->_cache[uri]);
	return (this->_cache[uri]->second);
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