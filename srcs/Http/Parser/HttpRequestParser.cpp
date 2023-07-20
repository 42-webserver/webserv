#include "../../../incs/Http/Parser/HttpRequestParser.hpp"


HttpRequestParser::HttpRequestParser(void)
: _httpRequest(new HttpRequest()), _state(BEFORE), _readBodySize(0)
{
	this->_buffer.reserve(_BUFFER_SIZE);
}

const RequestParseState &HttpRequestParser::parseRequest(std::vector<char> &reqBuffer, ft::shared_ptr<VirtualServerManager> vsm) 
	throw(HttpRequestParser::ClientBodySizeInvalidException){
	if (!this->_buffer.empty()) {
		reqBuffer.insert(reqBuffer.begin(), this->_buffer.begin(), this->_buffer.end());
		this->_buffer.clear();
	}
	if (_state == BEFORE || _state == START_LINE)
		handleStartLineState(reqBuffer);
	if (_state == HEADERS)
		handleHeaderState(reqBuffer, vsm);
	if (_state == BODY) 
		handleBodyState(reqBuffer);
	if (_state == FINISH){
		_state = BEFORE;
		_readBodySize = 0;
		return FINISH;
	}else
		return _state;
}

void HttpRequestParser::handleStartLineState(std::vector<char> &reqBuffer) {
	if (reqBuffer.empty())
		return;
	std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find == reqBuffer.end())
	{
		this->_state = START_LINE;
		this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
		return;
	}
	_httpRequest->setStartLine(std::string(reqBuffer.begin(), find));
	reqBuffer.erase(reqBuffer.begin(), find + _crlfPatternSize);
	this->_state = HEADERS;
}

void HttpRequestParser::handleHeaderState(std::vector<char> &reqBuffer, ft::shared_ptr<VirtualServerManager> vsm) {
	if (reqBuffer.empty())
		return;
	std::string line;
	std::vector<char>::iterator find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find == reqBuffer.end()) {
		this->_state = HEADERS;
		this->_buffer.insert(this->_buffer.end(), reqBuffer.begin(), reqBuffer.end());
		return;
	}
	while (find != reqBuffer.end()) {
		line = std::string(reqBuffer.begin(), find);
		reqBuffer.erase(reqBuffer.begin(), find + _crlfPatternSize);
		if (line.empty()) {
			changeStateToBody(vsm);
			return;
		}
		_httpRequest->addHeader(line);
		find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
}

void HttpRequestParser::changeStateToBody(ft::shared_ptr<VirtualServerManager> vsm) throw(HttpRequestParser::ClientBodySizeInvalidException){
	this->_state = BODY;
	int clientMaxBodySize = RouterUtils::findMaxBodySize(vsm, this->_httpRequest);
	injectionHandler(vsm);
	int contentLength = this->_httpRequest->getContentLength();
	if (contentLength > clientMaxBodySize)
		throw ClientBodySizeInvalidException();
}

void HttpRequestParser::injectionHandler(ft::shared_ptr<VirtualServerManager> vsm){
	std::multimap<std::string, std::string>::iterator it;
	std::multimap<std::string, std::string> _headers = this->_httpRequest->getHeaders();
	it = _headers.find("Content-Type");
	if (it != _headers.end()) {
		if (it->second.find("multipart/form-data") != std::string::npos) {
			std::string boundary = it->second.substr(it->second.find("boundary=") + 9);
			this->_bodyHandler = ft::make_shared<MultipartRequestBodyHandler>(boundary, vsm, this->_httpRequest);
			this->_httpRequest->setBodyType(MULTIPART_FORM_DATA);
			return;
		}
	}
	it = _headers.find("Transfer-Encoding");
	while (it != _headers.end()) {
		if (it->second == "chunked") {
			this->_httpRequest->setBodyType(CHUNKED);
			this->_bodyHandler =  ft::make_shared<ChunkedRequestBodyHandler>(vsm, this->_httpRequest);
			return;
		}
		it++;
	}
	this->_httpRequest->setBodyType(NORMAL);
	this->_bodyHandler = ft::make_shared<NormalBodyHandler>(this->_httpRequest);
}

void HttpRequestParser::handleBodyState(std::vector<char> &reqBuffer) {
	if (reqBuffer.empty())
		return;
	bool result;
	result = _bodyHandler->handleBody(reqBuffer);
	if(result)
		this->_state = FINISH;
}


const RequestParseState &HttpRequestParser::getState() {
	return this->_state;
}

const int &HttpRequestParser::getReadBodySize() {
	return this->_readBodySize;
}

const ft::shared_ptr<HttpRequest> HttpRequestParser::getHttpRequest() {
	return this->_httpRequest;
}

const std::vector<char> &HttpRequestParser::getBuffer() {
	return this->_buffer;
}

std::ostream &operator<<(std::ostream &os, const HttpRequestParser &parser) {
	os << "State: ";
	if (parser._state == BEFORE)
		os << "BEFORE" << std::endl;
	else if (parser._state == START_LINE)
		os << "START_LINE" << std::endl;
	else if (parser._state == HEADERS)
		os << "HEADERS" << std::endl;
	else if (parser._state == BODY)
		os << "BODY" << std::endl;
	else if (parser._state == FINISH)
		os << "FINISH" << std::endl;
	else
		os << "UNKNOWN" << std::endl;
	os << "ReadBodySize: " << parser._readBodySize << std::endl;
	os << "Buffer: " << std::endl;
	for (std::vector<char>::const_iterator it = parser._buffer.begin(); it != parser._buffer.end(); it++)
		os << *it;
	os << std::endl;
	os << "HttpRequest: " << std::endl;
	os << *(parser._httpRequest);
	return os;
}