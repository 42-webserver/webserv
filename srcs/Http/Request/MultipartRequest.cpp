#include "../../../incs/Http/Request/MultipartRequest.hpp"

MultipartRequest::MultipartRequest(void) {
	this->_body = ft::make_shared<IoReadAndWriteBuffer>();
}

MultipartRequest::~MultipartRequest(void) {}

void MultipartRequest::addHeader(const std::string & header)
{
	std::string::size_type pos;
	std::string line;

	pos = header.find(": ");
	if (pos == std::string::npos)
		return ;
	line = header.substr(pos + 2);
	handleMultipleValueHeader(line);
}

void MultipartRequest::handleMultipleValueHeader(std::string & line)
{
	std::string::size_type pos = line.find(";");
	while (pos != std::string::npos)
	{
		std::string tmp = line.substr(0, pos);
		if (tmp.find("=") != std::string::npos){
			std::string key = tmp.substr(0, tmp.find("="));
			std::string value = tmp.substr(tmp.find("=") + 1);
			this->_headers.insert(std::pair<std::string, std::string>(key, value));
			line.erase(0, pos + 1);
		}
		pos = line.find(";");
	}
	if (line.find("=") != std::string::npos){
		std::string key = line.substr(0, line.find("="));
		std::string value = line.substr(line.find("=") + 1);
		this->_headers.insert(std::pair<std::string, std::string>(key, value));
		line.erase(0, pos + 1);
	}
}

void MultipartRequest::insertBody(std::vector<char> &buffer) {
	this->_body->append(buffer.begin(), buffer.end());
}

ft::shared_ptr<IoReadAndWriteBuffer> MultipartRequest::getBody(void){ return (this->_body); }

bool MultipartRequest::needUpload(void){
	std::map<std::string, std::string>::iterator it = this->_headers.find("name");
	if (it == this->_headers.end() || it->second.compare("file") != 0)
		return false;
	it = this->_headers.find("filename");
	if (it == this->_headers.end())
		return false;
	return true;
}

std::string &MultipartRequest::getFilename(void){
	if (!needUpload())
		throw std::runtime_error("MultipartRequest::getFilename: this request is not file upload request");
	std::map<std::string, std::string>::iterator it = this->_headers.find("filename");
	return it->second;
}