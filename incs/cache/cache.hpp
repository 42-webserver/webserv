#ifndef CACHE_HPP
# define CACHE_HPP

#include <iostream>
#include <list>
#include <map>
#include <vector>

class LruCache {
private:
    int capacity;
    typedef std::list<std::pair<std::string, std::vector<char> > > lrulist_t;
    lrulist_t lru_list; 
    std::map<std::string, lrulist_t::iterator> cache; 

public:
    LruCache(int capacity);

    std::vector<char> get(std::string uri);
    void put(std::string uri, std::vector<char> content);

};

#endif
