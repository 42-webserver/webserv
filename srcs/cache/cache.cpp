#include "../../incs/cache/cache.hpp"

LruCache::LruCache(int capacity): capacity(capacity) {}
std::vector<char> LruCache::get(std::string uri) {
        if (cache.find(uri) == cache.end()) {
            // URI not in cache
            return std::vector<char>();
        } else {
            // URI in cache, move it to front
            lru_list.splice(lru_list.begin(), lru_list, cache[uri]);
            return cache[uri]->second;
        }
    }

void LruCache::put(std::string uri, std::vector<char> content) {
        if (cache.size() >= capacity) {
            // Cache is full, remove least recently used item
            lrulist_t::iterator last = lru_list.end();
            --last;
            cache.erase(last->first);
            lru_list.pop_back();
        }

        // Add new item to cache and update LRU list
        lru_list.push_front(std::make_pair(uri, content));
        cache[uri] = lru_list.begin();
    }