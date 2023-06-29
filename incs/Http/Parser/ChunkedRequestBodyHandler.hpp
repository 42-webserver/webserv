#ifndef CHUNKED_REQUEST_BODY_HANDLER_HPP
#define CHUNKED_REQUEST_BODY_HANDLER_HPP

#include "RequestBodyHandler.hpp"

class ChunkedRequestBodyHandler : public RequestBodyHandler
{

public:
class ChunkDataSizeNotMatchException : public std::exception {
	public:
			virtual const char* what() const throw();
};

private:
	std::vector<char>   _buffer;

public:
	bool handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req) throw(ChunkDataSizeNotMatchException);
	ChunkedRequestBodyHandler(void);
	~ChunkedRequestBodyHandler(void);

private:
	void writeInFile(std::vector<char> &writeBuffer, ft::shared_ptr<HttpRequest> req);
	void writeInMemory(std::vector<char> &writeBuffer, ft::shared_ptr<HttpRequest> req);
	int hexToDec(const std::string &hex);
};

#endif