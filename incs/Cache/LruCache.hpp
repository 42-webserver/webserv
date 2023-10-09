#ifndef LRUCACHE_HPP
# define LRUCACHE_HPP

# include <iostream>
# include <list>
# include <map>
# include <vector>

class LruCache {
	public:
	class FailToGetException: public std::exception {
		public:
			virtual const char *what() const throw();
	};
	public:
		typedef std::list< std::pair< std::string, std::vector< char> > > lru_list_t;
		typedef std::map< std::string, lru_list_t::iterator > cache_map_t;

	private:
		unsigned long			_capacity;
		lru_list_t	_lru_list; 
		std::vector<char>	empty;
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
