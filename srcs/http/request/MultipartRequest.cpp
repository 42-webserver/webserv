#include "../../../incs/http/request/MultipartRequest.hpp"

MultipartRequest::MultipartRequest(void)
{
}

MultipartRequest::~MultipartRequest(void)
{
}


std::string & MultipartRequest::getBodyDataFilename(){
	return this->_bodyDataFilename;
}


void MultipartRequest::addHeader(const std::string & header)
{
	std::string::size_type pos;
	std::string key;
	std::string value;

	pos = header.find(": ");
	key = header.substr(0, pos);
	value = header.substr(pos + 2);
	handleMultipleValueHeader(value, key);
}

void MultipartRequest::handleMultipleValueHeader(std::string & value, std::string & key)
{
	std::string::size_type pos = value.find(";");

	while (pos != std::string::npos)
	{
		this->_headers.insert(std::pair<std::string, std::string>(key, value.substr(0, pos)));
		value.erase(0, pos + 1);
		pos = value.find(";");
	}
	this->_headers.insert(std::pair<std::string, std::string>(key, value));
}

void MultipartRequest::insertBody(std::vector<char> &buffer)
{
	this->_memoryBody.insert(this->_memoryBody.end(), buffer.begin(), buffer.end());
}

void MultipartRequest::setBodyDataFilename(std::string filename){
	this->_bodyDataFilename = filename;
}

std::multimap<std::string, std::string> &MultipartRequest::getHeaders(){
	return this->_headers;
}

std::vector<char> &MultipartRequest::getMemoryBody(){
	return this->_memoryBody;
}