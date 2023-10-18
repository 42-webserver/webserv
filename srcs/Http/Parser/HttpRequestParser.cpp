#include "../../../incs/Http/Parser/HttpRequestParser.hpp"


HttpRequestParser::HttpRequestParser(void)
: _state(BEFORE)
{
	this->_httpRequest = ft::make_shared<HttpRequest>();
	this->_buffer.reserve(_BUFFER_SIZE);
}

const RequestParseState &HttpRequestParser::parseRequest(ft::shared_ptr<VirtualServerManager> vsm) {
	IoOnlyReadBuffer &readBuffer = IoOnlyReadBuffer::getInstance();
	this->_buffer.insert(this->_buffer.end(), readBuffer.begin(), readBuffer.end());
	if (_state == BEFORE || _state == START_LINE)
		handleStartLineState();
	if (_state == HEADERS)
		handleHeaderState(vsm);
	if (_state == BODY) 
		handleBodyState();
	return _state;
}

void HttpRequestParser::handleStartLineState() {
	if (_buffer.empty())
		return;
	std::vector<char>::iterator find = std::search(_buffer.begin(), _buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find == _buffer.end()) {
		this->_state = START_LINE;
		return;
	}
	_httpRequest->setStartLine(std::string(_buffer.begin(), find));
	_buffer.erase(_buffer.begin(), find + _crlfPatternSize);
	this->_state = HEADERS;
}

void HttpRequestParser::handleHeaderState(ft::shared_ptr<VirtualServerManager> vsm) {
	if (_buffer.empty())
		return;
	std::string line;
	std::vector<char>::iterator find = std::search(_buffer.begin(), _buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	if (find == _buffer.end()) {
		this->_state = HEADERS;
		return;
	}
	while (find != _buffer.end()) {
		line = std::string(_buffer.begin(), find);
		_buffer.erase(_buffer.begin(), find + _crlfPatternSize);
		if (line.empty()) {
			changeStateToBody(vsm);
			return;
		}
		_httpRequest->addHeader(line);
		find = std::search(_buffer.begin(), _buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
}

void HttpRequestParser::changeStateToBody(ft::shared_ptr<VirtualServerManager> vsm){
	this->_state = BODY;
	int clientMaxBodySize = RouterUtils::findMaxBodySize(vsm, this->_httpRequest);
	injectionHandler();
	int contentLength = this->_httpRequest->getContentLength();
	if (contentLength > clientMaxBodySize){
		this->_httpRequest->setError(REQUEST_ENTITY_TOO_LARGE);
		this->_state = FINISH;
	}
}

void HttpRequestParser::injectionHandler(){
	std::multimap<std::string, std::string>::iterator it;
	std::multimap<std::string, std::string> _headers = this->_httpRequest->getHeaders();
	it = _headers.find("Content-Type");
	if (it != _headers.end()) {
		if (it->second.find("multipart/form-data") != std::string::npos) {
			std::string boundary = it->second.substr(it->second.find("boundary=") + 9);
			this->_bodyHandler = ft::make_shared<MultipartRequestBodyHandler>(boundary, this->_httpRequest);
			this->_httpRequest->setBodyType(MULTIPART_FORM_DATA);
			return;
		}
	}
	it = _headers.find("Transfer-Encoding");
	while (it != _headers.end()) {
		if (it->second == "chunked") {
			this->_httpRequest->setBodyType(CHUNKED);
			this->_bodyHandler =  ft::make_shared<ChunkedRequestBodyHandler>(this->_httpRequest);
			return;
		}
		it++;
	}
	this->_httpRequest->setBodyType(NORMAL);
	this->_bodyHandler = ft::make_shared<NormalBodyHandler>(this->_httpRequest);
}

void HttpRequestParser::handleBodyState() {
	if (_buffer.empty())
		return;
	bool result;
	result = _bodyHandler->handleBody(_buffer);
	if(result)
		this->_state = FINISH;
}


const RequestParseState &HttpRequestParser::getState() {
	return this->_state;
}

const ft::shared_ptr<HttpRequest> HttpRequestParser::getHttpRequest() {
	ft::shared_ptr<HttpRequest> tmp = this->_httpRequest;
	this->_httpRequest = ft::make_shared<HttpRequest>();
	this->_state = BEFORE;
	return tmp;
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
	os << "Buffer: " << std::endl;
	for (std::vector<char>::const_iterator it = parser._buffer.begin(); it != parser._buffer.end(); it++)
		os << *it;
	os << std::endl;
	os << "HttpRequest: " << std::endl;
	os << *(parser._httpRequest);
	return os;
}