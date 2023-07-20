#include "../../../incs/Http/Parser/NormalBodyHandler.hpp"


NormalBodyHandler::NormalBodyHandler(ft::shared_ptr<HttpRequest> req) : RequestBodyHandler(0, req)
{}

NormalBodyHandler::~NormalBodyHandler(void)
{}

bool NormalBodyHandler::handleBody(std::vector<char> &reqBuffer)
{
	writeInMemory(reqBuffer);
	if (this->_request->getContentLength() <= this->_readBodySize)
		return true;
	return false;
}

void NormalBodyHandler::writeInMemory(std::vector<char> &reqBuffer)
{
	this->_request->insertBody(reqBuffer);
	this->_readBodySize += reqBuffer.size();
}