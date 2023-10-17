#ifndef LRUCACHE_HPP
# define LRUCACHE_HPP

# include <iostream>
# include <list>
# include <map>
# include <vector>
# include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
# include "../../../libs/shared_ptr/shared_ptr.hpp"
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
		static const int _capacity = 4 * 1024;
		lru_list_t	_lru_list; 
		std::vector<char>	empty;
		cache_map_t	_cache; 
	public:
		LruCache(void);
		~LruCache(void);

	public:
		const std::vector<char>	&get(const std::string &uri);
		std::vector<char>::iterator	getIter(const std::string &uri);
		 size_t				getCacheContentSize(const std::string &uri);
		void				put(std::string uri, std::vector<char> content);
		void				put(const std::string& uri);
		void 				put(const std::string& uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer);
};

#endif
