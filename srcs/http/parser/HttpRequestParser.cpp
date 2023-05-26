#include "../../../incs/http/parser/HttpRequestParser.hpp"


HttpRequestParser::HttpRequestParser(HttpRequest *httpRequest)
: _httpRequest(httpRequest), _state(BEFORE), _readBodySize(0)
{
}

void HttpRequestParser::parseRequest(std::vector<char> &buffer) {
    if (!this->_buffer.empty()){
        buffer.insert(buffer.begin(), this->_buffer.begin(), this->_buffer.end());
        this->_buffer.clear();
    }
    if (_state == BEFORE || _state == START_LINE)
        handleStartLineState(buffer);
    if (_state == HEADERS)
        handleHeaderState(buffer);
    if (_state == BODY)
        handleBodyState(buffer);
}

void HttpRequestParser::handleStartLineState(std::vector<char> &buffer) {
    if (buffer.empty())
        return;
    std::vector<char>::iterator find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    if (find == buffer.end())
    {
        this->_state = START_LINE;
        this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
        return;
    }
    _httpRequest->setStartLine(std::string(buffer.begin(), find));
    buffer.erase(buffer.begin(), find + _crlfPatternSize);
    this->_state = HEADERS;
}

void HttpRequestParser::handleHeaderState(std::vector<char> &buffer) {
    if (buffer.empty())
        return;
    std::string line;
    std::vector<char>::iterator find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    if (find == buffer.end())
    {
        this->_state = HEADERS;
        this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
        return;
    }
    while(find != buffer.end()){
        line = std::string(buffer.begin(), find);
        if (line.empty())
        {
            buffer.erase(buffer.begin(), find + _crlfPatternSize);
            this->_state = BODY;
            return;
        }
        _httpRequest->addHeader(line);
        buffer.erase(buffer.begin(), find + _crlfPatternSize);
        find = std::search(buffer.begin(), buffer.end(), _crlfPattern.begin(), _crlfPattern.end());
    }
    this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
}

void HttpRequestParser::handleBodyState(std::vector<char> &buffer) {
    if (buffer.empty())  
        return;
    this->_buffer.insert(this->_buffer.end(), buffer.begin(), buffer.end());
    _readBodySize += buffer.size();
}

RequestParseState HttpRequestParser::getState() {
    return this->_state;
}

int HttpRequestParser::getReadBodySize() {
    return this->_readBodySize;
}

HttpRequest *HttpRequestParser::getHttpRequest() {
    return this->_httpRequest;
}

std::vector<char> HttpRequestParser::getBuffer() {
    return this->_buffer;
}