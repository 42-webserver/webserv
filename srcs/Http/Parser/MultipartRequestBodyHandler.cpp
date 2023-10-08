#include "../../../incs/Http/Parser/MultipartRequestBodyHandler.hpp"

MultipartRequestBodyHandler::MultipartRequestBodyHandler(std::string boundary, ft::shared_ptr<HttpRequest> req)
	: RequestBodyHandler(0, req), _state(M_HEADER), _index(0){
	this->_boundaryStart = "--" + boundary;
	this->_boundaryEnd =  "--" + boundary + "--";
}

MultipartRequestBodyHandler::~MultipartRequestBodyHandler(void){}

bool MultipartRequestBodyHandler::handleBody(std::vector<char> &buffer){
    bool result = false;
	int contentLength = this->_request->getContentLength();
    if (buffer.size() >= static_cast<size_t>(contentLength)){
		std::vector<char> tmp(buffer.begin(), buffer.begin() + contentLength);
		buffer.erase(buffer.begin(), buffer.begin() + contentLength);
		parseParts(tmp);
		result = true;
	}
    return result;
}

void MultipartRequestBodyHandler::parseParts(std::vector<char> &partBody){
	allocateMultiparts(countPartSize(partBody));
	while (_state != M_END){
		if (_state == M_HEADER)
			handleMultipartHeader(partBody);
		if (_state == M_BODY){
			parsePartOfBody(partBody);
		}
	}
}

void MultipartRequestBodyHandler::allocateMultiparts(int count){
	std::vector<MultipartRequest> multiparts = this->_request->getMultipartRequests();
	for (int i = 0; i < count; i++){
		multiparts.push_back(MultipartRequest());
	}
}

int MultipartRequestBodyHandler::countPartSize(std::vector<char> &partBody){
	int count = 0;
	std::vector<char>::iterator find = std::search(partBody.begin(), partBody.end(), _boundaryStart.begin(), _boundaryStart.end());
	while (find != partBody.end()){
		count++;
		find = std::search(find + _boundaryStart.size(), partBody.end(), _boundaryStart.begin(), _boundaryStart.end());
	}
	return count;
}

void MultipartRequestBodyHandler::handleMultipartHeader(std::vector<char> &partBody){
	std::vector<char>::iterator find = std::search(partBody.begin(), partBody.end(), _crlfPattern.begin(), _crlfPattern.end());
	std::string line;
	while (find != partBody.end()){
		line = std::string(partBody.begin(), find);
		partBody.erase(partBody.begin(), find + _crlfPatternSize);
		if (line == _boundaryStart){
			find = std::search(partBody.begin(), partBody.end(), _crlfPattern.begin(), _crlfPattern.end());
			continue;
		}
		if (line.empty()){
			_state = M_BODY;
			return;
		}
		addHeader(line);
		find = std::search(partBody.begin(), partBody.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
}


void MultipartRequestBodyHandler::parsePartOfBody(std::vector<char> &partBody){
	std::vector<char>::iterator find = std::search(partBody.begin(), partBody.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find != partBody.end()){
		std::vector<char> part(partBody.begin(), find);
		partBody.erase(partBody.begin(), find + _crlfPatternSize);
		addBody(part);
		this->_index++;
		this->_state = M_HEADER;
	}
	find = std::search(partBody.begin(), partBody.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find != partBody.end() && std::string(partBody.begin(), find) == _boundaryEnd){
		partBody.erase(partBody.begin(), find + _crlfPatternSize);
		_state = M_END;
	}
}

void MultipartRequestBodyHandler::addBody(std::vector<char> &body){
	MultipartRequest &multipartRequest = this->_request->getMultipartRequests()[this->_index];
	multipartRequest.insertBody(body);
}

void MultipartRequestBodyHandler::addHeader(const std::string & line)
{
	MultipartRequest &multipartRequest = this->_request->getMultipartRequests()[this->_index];
	multipartRequest.addHeader(line);
	
}
