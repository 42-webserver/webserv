#ifndef CHUNKED_REQUEST_BODY_HANDLER_HPP
#define CHUNKED_REQUEST_BODY_HANDLER_HPP

#include "RequestBodyHandler.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Utils/RouterUtils.hpp"

class ChunkedRequestBodyHandler : public RequestBodyHandler
{

public:
class ChunkDataSizeNotMatchException : public std::exception {
	public:
			virtual const char* what() const throw();
};

private:
	ft::shared_ptr<HttpRequest> 			_request;

public:
	bool handleBody(std::vector<char> &buffer) throw(ChunkDataSizeNotMatchException);
	ChunkedRequestBodyHandler(ft::shared_ptr<HttpRequest> req);
	~ChunkedRequestBodyHandler(void);

private:
	void _writeInMemory(std::vector<char> &writeBuffer);
	int _hexToDec(const std::string &hex);
};

#endif