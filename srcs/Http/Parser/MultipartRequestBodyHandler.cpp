#include "../../../incs/Http/Parser/MultipartRequestBodyHandler.hpp"

MultipartRequestBodyHandler::MultipartRequestBodyHandler(std::string boundary)
	: RequestBodyHandler(0), _state(M_HEADER) {
	this->_boundaryStart = "--" + boundary;
	this->_boundaryEnd =  "--" + boundary + "--";
}

MultipartRequestBodyHandler::~MultipartRequestBodyHandler(void){}

bool MultipartRequestBodyHandler::handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
	if (!this->_buffer.empty()) {
		reqBuffer.insert(reqBuffer.begin(), this->_buffer.begin(), this->_buffer.end());
		this->_buffer.clear();
	}
    bool result = false;
    while (!reqBuffer.empty()){
        if (this->_state == M_HEADER)
            handleMultipartHeader(reqBuffer, req);
        if (this->_state == M_BODY)
            result = parsePartOfBody(reqBuffer);
    }
    return result;
}

void MultipartRequestBodyHandler::handleMultipartHeader(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req){
	std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	std::string line;
	while (find != reqBuffer.end()){
		line = std::string(reqBuffer.begin(), find);
		reqBuffer.erase(reqBuffer.begin(), find + _crlfPatternSize);
		if (line == _boundaryStart){
			find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
			continue;
		}
		if (line.empty()){
			this->_state = M_BODY;
            // make full path to upload
            req->getBody();
            generatePath();
            FileUploader::checkFileExists(this->_multipartRequest._filename);
			return;
		}
		addHeader(line);
		find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
	this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
	reqBuffer.clear();
}

void MultipartRequestBodyHandler::generatePath(){
    std::string path = "/Users/leehyunkyu/Desktop/upload/";
    std::multimap<std::string, std::string>::iterator mapIt = this->_multipartRequest._headers.find("Content-Disposition");
    if (mapIt == this->_multipartRequest._headers.end())
        throw FileUploader::FileUploadException("Content-Disposition header not found");
    while(mapIt != this->_multipartRequest._headers.end()){
        if (mapIt->second.find("filename=") == std::string::npos){
            mapIt++;
            continue;
        }
        std::string filename = mapIt->second.substr(mapIt->second.find("filename=") + 9);
        filename = std::string(filename.begin() + 1, filename.end() - 1);
        if (filename.empty())
            throw FileUploader::FileUploadException("filename is empty");
        this->_multipartRequest._filename = path + filename;
        break;
    }
}

bool MultipartRequestBodyHandler::parsePartOfBody(std::vector<char> &reqBuffer){
	std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find != reqBuffer.end()){
		std::vector<char> part(reqBuffer.begin(), find);
		reqBuffer.erase(reqBuffer.begin(), find + _crlfPatternSize);
        FileUploader::fileUpload(part, this->_multipartRequest._filename);
		this->_state = M_HEADER;
        this->_multipartRequest._headers.clear();
        this->_multipartRequest._filename.clear();
	}else{
        FileUploader::fileUpload(reqBuffer, this->_multipartRequest._filename);
		reqBuffer.clear();
	}
	find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find != reqBuffer.end() && std::string(reqBuffer.begin(), find) == _boundaryEnd){
		reqBuffer.clear();
		return true;
	}
	return false;
}

void MultipartRequestBodyHandler::addHeader(const std::string & line)
{
	std::string::size_type pos;
	std::string key;
	std::string value;

	pos = line.find(": ");
	key = line.substr(0, pos);
	value = line.substr(pos + 2);
	handleMultipleValueHeader(value, key);
}

void MultipartRequestBodyHandler::handleMultipleValueHeader(std::string & value, std::string & key)
{
	std::string::size_type pos = value.find(";");

	while (pos != std::string::npos)
	{
		this->_multipartRequest._headers.insert(std::pair<std::string, std::string>(key, value.substr(0, pos)));
		value.erase(0, pos + 1);
		pos = value.find(";");
	}
	this->_multipartRequest._headers.insert(std::pair<std::string, std::string>(key, value));
}