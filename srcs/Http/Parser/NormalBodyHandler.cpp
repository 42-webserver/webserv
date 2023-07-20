#include "../../../incs/Http/Parser/NormalBodyHandler.hpp"


NormalBodyHandler::NormalBodyHandler(ft::shared_ptr<HttpRequest> req) : RequestBodyHandler(0, req)
{}

NormalBodyHandler::~NormalBodyHandler(void)
{}

bool NormalBodyHandler::handleBody(std::vector<char> &buffer)
{
	int contentLength = this->_request->getContentLength();
	int writeSize = contentLength - this->_readBodySize;
	writeInMemory(buffer, writeSize);
	if (contentLength <= this->_readBodySize)
		return true;
	return false;
}

void NormalBodyHandler::writeInMemory(std::vector<char> &buffer, int writeSize)
{
	std::vector<char> tmp(buffer.begin(), buffer.begin() + writeSize);
	this->_request->insertBody(tmp);
	this->_readBodySize += tmp.size();
	buffer.erase(buffer.begin(), buffer.begin() + tmp.size());
}
