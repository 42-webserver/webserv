#ifndef CHUNKED_REQUEST_BODY_HANDLER_HPP
#define CHUNKED_REQUEST_BODY_HANDLER_HPP

#include "RequestBodyHandler.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Utils/FileUploader.hpp"
#include "../Utils/RouterUtils.hpp"

class ChunkedRequestBodyHandler : public RequestBodyHandler
{

public:
class ChunkDataSizeNotMatchException : public std::exception {
	public:
			virtual const char* what() const throw();
};

private:
	std::vector<char>   					_buffer;
	ft::shared_ptr<VirtualServerManager>	_vsm;
	std::string								_filename;
	ft::shared_ptr<HttpRequest> 			_request;

public:
	bool handleBody(std::vector<char> &reqBuffer) throw(ChunkDataSizeNotMatchException);
	ChunkedRequestBodyHandler(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
	~ChunkedRequestBodyHandler(void);

private:
	void _generateFilename(ft::shared_ptr<HttpRequest> req);
	void writeInFile(std::vector<char> &writeBuffer, ft::shared_ptr<HttpRequest> req);
	void writeInMemory(std::vector<char> &writeBuffer, ft::shared_ptr<HttpRequest> req);
	int _hexToDec(const std::string &hex);
	void _uploadFile(std::vector<char> &buffer);
};

#endif