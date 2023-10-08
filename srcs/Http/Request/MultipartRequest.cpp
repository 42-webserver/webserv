#include "../../../incs/Http/Request/MultipartRequest.hpp"

MultipartRequest::MultipartRequest(void) {}

MultipartRequest::~MultipartRequest(void) {}

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

void MultipartRequest::insertBody(std::vector<char> &buffer) {
	this->_body.append(buffer.begin(), buffer.end());
}

std::multimap<std::string, std::string> &MultipartRequest::getHeaders(){ return (this->_headers); }

IoReadAndWriteBuffer &MultipartRequest::getBody(){ return (this->_body); }