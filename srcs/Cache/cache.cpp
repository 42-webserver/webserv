#include "../../incs/Cache/cache.hpp"
#include <fstream>
Cache *Cache::_instance = NULL;
Cache::Cache(void) {}
Cache::~Cache(void) {}


void Cache::deleteInstance(void) {
	if (Cache::_instance != NULL) {
		delete Cache::_instance;
		Cache::_instance = NULL;
	}
}
Cache	&Cache::getInstance(void) {
	if (Cache::_instance == NULL) {
		Cache::_instance = new Cache();
	}
	return (*Cache::_instance);
}
bool Cache::checkAuthentification(const std::string &uri) {
	(void)uri;
	//TODO: check authentification
	return (true);
}
void Cache::checkAndThrow(const std::string &uri){
 	std::fstream file;
    file.open("test.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	if (!file.is_open()) {
		throw FileNoExistException();
	}
	if (!checkAuthentification(uri)) {
		throw FileAuthentificationException();
	}
	file.seekg(0, std::ios::end);
	if (file.tellg() > Cache::cacheBufferSize) {
		throw BigFileException();
	}
	file.close();
}

const std::vector<char>	&Cache::getFileContent(const std::string &uri) {
	checkAndThrow(uri);
	// this->_cache.get(uri)
	return (this->_cache.get(uri));
}
const char *Cache::BigFileException::what() const throw() {
	return ("File is too big");
}
const char *Cache::FileAuthentificationException::what() const throw() {
	return ("File authentification failed");
}
const char *Cache::FileNoExistException::what() const throw() {
	return ("File does not exist");
}