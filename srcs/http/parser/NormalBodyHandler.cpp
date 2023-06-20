#include "../../../incs/http/parser/NormalBodyHandler.hpp"


NormalBodyHandler::NormalBodyHandler(void) : RequestBodyHandler(0)
{}

NormalBodyHandler::~NormalBodyHandler(void)
{}

bool NormalBodyHandler::handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req)
{
	if (req->isBodyLong())
		writeInFile(reqBuffer, req);
	else
		writeInMemory(reqBuffer, req);
	reqBuffer.clear();

	if (req->getContentLength() <= this->_readBodySize)
		return true;
	return false;
}

void  NormalBodyHandler::writeInFile(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req)
{
	if (req->getBodyDataFilename().empty())
		req->setBodyDataFilename(FileNameGenerator::generateUniqueFileName());
	std::string fileName = req->getBodyDataFilename();
	std::ofstream file(fileName.c_str(), std::ios::app);
	if (!file.is_open())
		throw std::runtime_error("Error: can't open file");
	file.write(reqBuffer.data(), reqBuffer.size());
	file.close();
	this->_readBodySize += reqBuffer.size();
}

void NormalBodyHandler::writeInMemory(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req)
{
	req->insertBody(reqBuffer);
	this->_readBodySize += reqBuffer.size();
}