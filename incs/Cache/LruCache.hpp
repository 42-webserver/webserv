#ifndef LRUCACHE_HPP
# define LRUCACHE_HPP

# include <iostream>
# include <list>
# include <map>
# include <vector>

class LruCache {
	public:
		typedef std::list< std::pair< std::string, std::vector< char> > > lru_list_t;
		typedef std::map< std::string, lru_list_t::iterator > cache_map_t;

	private:
		int			_capacity;
		lru_list_t	_lru_list; 
		cache_map_t	_cache; 
		void		chekcFileSize(const std::string &Filepath);
	public:
		LruCache(void);
		~LruCache(void);
		LruCache(int capacity);

	public:
		const std::vector<char>	&get(const std::string &uri);
		void				put(std::string uri, std::vector<char> content);
};

#endif
