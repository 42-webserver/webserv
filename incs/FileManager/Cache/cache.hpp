#ifndef CACHE_HPP
# define CACHE_HPP
#include "LruCache.hpp"
#include "../../Http/Exception/BadRequestException.hpp"
#include "../../Http/Exception/ServerErrorException.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

class Cache{
public:
		static Cache	&getInstance(void);
		void deleteInstance(void);
		// size_t	copyFromCacheTo(IoReadAndWriteBuffer &buffer, const std::string &uri);
		size_t  getCacheContentSize(const std::string &uri);
		void    getCacheContent(const std::string &uri, std::vector<char> &buffer);
		void    putCacheContent(const std::string &uri);
		void	putCacheContent(const std::string &uri, ft::shared_ptr<IoReadAndWriteBuffer> buffer);
		void	deleteCacheContent(const std::string &uri);
public:		
		bool hit(const std::string &uri);
private:
	Cache(void);
	~Cache(void);

	//delete
private:
	Cache(const Cache &ref);
	Cache &operator=(const Cache &rhs);
private:
	LruCache	_cache;
	static Cache	*_instance;
public:
	static const int cache_block_size = 1024 * 4;
};

#endif