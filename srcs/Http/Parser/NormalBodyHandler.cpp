#include "../../../incs/Http/Parser/NormalBodyHandler.hpp"


NormalBodyHandler::NormalBodyHandler(ft::shared_ptr<HttpRequest> req) : RequestBodyHandler(0, req)
{}

NormalBodyHandler::~NormalBodyHandler(void)
{}

bool NormalBodyHandler::handleBody(std::vector<char> &reqBuffer)
{
	if (this->_request->isBodyLong())
		writeInFile(reqBuffer);
	else
		writeInMemory(reqBuffer);
	reqBuffer.clear();

	if (this->_request->getContentLength() <= this->_readBodySize)
		return true;
	return false;
}

void  NormalBodyHandler::writeInFile(std::vector<char> &reqBuffer)
{
	if (this->_request->getBodyDataFilename().empty())
		this->_request->setBodyDataFilename(FileNameGenerator::generateUniqueFileName());
	std::string fileName = this->_request->getBodyDataFilename();
	std::ofstream file(fileName.c_str(), std::ios::app);
	if (!file.is_open())
		throw std::runtime_error("Error: can't open file");
	file.write(reqBuffer.data(), reqBuffer.size());
	file.close();
	this->_readBodySize += reqBuffer.size();
}

void NormalBodyHandler::writeInMemory(std::vector<char> &reqBuffer)
{
	this->_request->insertBody(reqBuffer);
	this->_readBodySize += reqBuffer.size();
}