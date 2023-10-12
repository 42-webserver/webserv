#ifndef CACHE_HPP
# define CACHE_HPP
#include "LruCache.hpp"
#include "../../Http/Exception/BadRequestException.hpp"
#include "../../Http/Exception/ServerErrorException.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
class Cache{
public:
	static const int	cacheBufferSize = 1024 *4;
public:
		static Cache	&getInstance(void);
		void deleteInstance(void);
		size_t	copyFromCacheTo(IoReadAndWriteBuffer &buffer, const std::string &uri) const;
		size_t  getCacheContentSize(const std::string &uri) const;
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
};

#endif