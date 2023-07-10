#include "../../../incs/Http/Parser/HttpRequestParser.hpp"


HttpRequestParser::HttpRequestParser(void)
: _httpRequest(new HttpRequest()), _state(BEFORE), _readBodySize(0)
{
	this->_buffer.reserve(_BUFFER_SIZE);
}

const RequestParseState &HttpRequestParser::parseRequest(std::vector<char> &reqBuffer, int clientBodyMaxSize) {
	if (!this->_buffer.empty()) {
		reqBuffer.insert(reqBuffer.begin(), this->_buffer.begin(), this->_buffer.end());
		this->_buffer.clear();
	}
	if (_state == BEFORE || _state == START_LINE)
		handleStartLineState(reqBuffer);
	if (_state == HEADERS)
		handleHeaderState(reqBuffer, clientBodyMaxSize);
	if (_state == BODY) 
		handleBodyState(reqBuffer);
	return this->_state;
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

void HttpRequestParser::handleHeaderState(std::vector<char> &reqBuffer, int clientBodyMaxSize) {
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
			changeStateToBody(clientBodyMaxSize);
			return;
		}
		_httpRequest->addHeader(line);
		find = std::search(reqBuffer.begin(), reqBuffer.end(), _crlfPattern.begin(), _crlfPattern.end());
	}
}

void HttpRequestParser::changeStateToBody(int clientBodyMaxSize) throw(HttpRequestParser::ClientBodySizeInvalidException){
	this->_state = BODY;
	injectionHandler();
	int contentLength = this->_httpRequest->getContentLength();
	if (contentLength > _MAX_BODY_MEMORY_SIZE)
		this->_httpRequest->setBodyLong(true);
	if (contentLength > clientBodyMaxSize)
		throw ClientBodySizeInvalidException();
}

void HttpRequestParser::injectionHandler(){
	std::multimap<std::string, std::string>::iterator it;
	std::multimap<std::string, std::string> _headers = this->_httpRequest->getHeaders();
	it = _headers.find("Content-Type");
	if (it != _headers.end()) {
		if (it->second.find("multipart/form-data") != std::string::npos) {
			std::string boundary = it->second.substr(it->second.find("boundary=") + 9);
			this->_bodyHandler = ft::shared_ptr<RequestBodyHandler>(new MultipartRequestBodyHandler(boundary));
			this->_httpRequest->setBodyType(MULTIPART_FORM_DATA);
			return;
		}
	}
	it = _headers.find("Transfer-Encoding");
	while (it != _headers.end()) {
		if (it->second == "chunked") {
			this->_httpRequest->setBodyType(CHUNKED);
			this->_bodyHandler =  ft::shared_ptr<RequestBodyHandler>(new ChunkedRequestBodyHandler());
			return;
		}
		it++;
	}
	this->_httpRequest->setBodyType(NORMAL);
	this->_bodyHandler = ft::shared_ptr<RequestBodyHandler>(new NormalBodyHandler());
}

void HttpRequestParser::handleBodyState(std::vector<char> &reqBuffer) {
	if (reqBuffer.empty())
		return;
	bool result;
	result = _bodyHandler->handleBody(reqBuffer, _httpRequest);
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

const char * HttpRequestParser::ClientBodySizeInvalidException::what() const throw() { 
	return ("HttpRequestParse: client max body size is invalid");
}