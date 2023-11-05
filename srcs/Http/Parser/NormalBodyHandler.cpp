#include "../../../incs/Http/Parser/NormalBodyHandler.hpp"
#include <Buffer/Buffer/IoOnlyReadBuffer.hpp>


NormalBodyHandler::NormalBodyHandler(ft::shared_ptr<HttpRequest> req) : RequestBodyHandler(0, req)
{}

NormalBodyHandler::~NormalBodyHandler(void)
{}

bool NormalBodyHandler::handleBody(std::vector<char> &buffer)
{
	ssize_t contentLength = this->_request->getContentLength();
	// int writeSize = contentLength - this->_readBodySize;
	writeInMemory(buffer);
	if (contentLength <= this->_readBodySize)
		return true;
	std::cerr << "normal -- contentLength: " << contentLength << std::endl;
	std::cerr << "normal -- this->_readBodySize: " << this->_readBodySize << std::endl;
	return false;
}

// void NormalBodyHandler::writeInMemory(std::vector<char> &buffer, int writeSize)
// {
// 	// ssize_t n;
// 	std::vector<char> tmp(buffer.begin(), buffer.begin() + writeSize);
// 	// if (buffer.empty()) {
// 	// 	n = this->_request->insertBody(buffer);
// 	// 	buffer.clear();
// 	// 	this->_readBodySize += n;
// 	// }
// 	// IoOnlyReadBuffer &readBuffer = IoOnlyReadBuffer::getInstance();

// 	// if (readBuffer.size() > 0)
// 	// {
// 	// 	n = this->_request->insertBody();
// 	// 	this->_readBodySize += n;
// 	// }
// 	this->_request->insertBody(tmp);
// 	this->_readBodySize += tmp.size();
// 	buffer.erase(buffer.begin(), buffer.begin() + tmp.size());
// }
void NormalBodyHandler::writeInMemory(std::vector<char> &buffer)
{
	ssize_t n;
	if (buffer.empty()) {
		n = this->_request->insertBody(buffer);
		buffer.clear();
		this->_readBodySize += n;
	}
	IoOnlyReadBuffer &readBuffer = IoOnlyReadBuffer::getInstance();

	if (readBuffer.size() > 0)
	{
		n = this->_request->insertBody();
		this->_readBodySize += n;
	}
}
