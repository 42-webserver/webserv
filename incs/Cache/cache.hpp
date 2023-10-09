#ifndef CACHE_HPP
# define CACHE_HPP
#include "LruCache.hpp"
#include "../Http/Exception/BadRequestException.hpp"
#include "../Http/Exception/ServerErrorException.hpp"

class Cache{
public:
	static const int	cacheBufferSize = 1024 *4;
public:
class BigFileException {
	public:
		virtual const char *what() const throw();
};
class FileAuthentificationException: public BadRequestException {
	public:
		FileAuthentificationException(void) : BadRequestException(FORBIDDEN){};
		virtual const char *what() const throw();
};
class FileNoExistException: public ServerErrorException {
	public:
		FileNoExistException(void) : ServerErrorException(INTERNAL_SERVER_ERROR){};
	public:
		virtual const char *what() const throw();
};
public:
		static Cache	&getInstance(void);
		void deleteInstance(void);
		const std::vector<char>	&getFileContent(const std::string &uri) throw(BigFileException, FileAuthentificationException,
		FileNoExistException);
private:
	Cache(void);
	~Cache(void);
	Cache(const Cache &ref);
	Cache &operator=(const Cache &rhs);
	void checkAndThrow(const std::string &uri) throw(BigFileException, FileAuthentificationException,
		FileNoExistException);
	bool checkAuthentification(const std::string &uri);
private:
	LruCache	_cache;
	static Cache	*_instance;
};

#endif