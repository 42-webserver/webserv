#include "../../../incs/http/parser/HttpRequestParser.hpp"


HttpRequestParser::HttpRequestParser(HttpRequest *httpRequest)
: _httpRequest(httpRequest), _state(BEFORE), _readBodySize(0)
{
}

void HttpRequestParser::parseRequest(const char *buffer) {
     std::string *bufferStr = new std::string(buffer);
    if (!this->_buffer.empty()){
        bufferStr->insert(0, this->_buffer);
        this->_buffer.clear();
    }
    if (_state == BEFORE || _state == START_LINE)
        handleStartLineState(bufferStr);
    if (_state == HEADERS)
        handleHeaderState(bufferStr);
    if (_state == BODY)
        handleBodyState(bufferStr);
}

void HttpRequestParser::handleStartLineState(std::string *bufferStr) {
    if (bufferStr->empty())
        return;
    std::string::size_type pos = bufferStr->find("\r\n");
    std::string line = bufferStr->substr(0, pos);
    
    if(pos == std::string::npos){
        this->_state = START_LINE;
        this->_buffer = *bufferStr;
        return;
    }
    _httpRequest->setStartLine(line);
    bufferStr->erase(0, pos + 2);
    this->_state = HEADERS;
}

void HttpRequestParser::handleHeaderState(std::string *bufferStr) {
    std::string::size_type pos;
    std::string line;
    if (bufferStr->empty())  
        return;
    pos = bufferStr->find("\r\n");
    while(pos != std::string::npos){
        line = bufferStr->substr(0, pos);
        if (line.empty())
        {
            bufferStr->erase(0, pos + 2);
            this->_state = BODY;
            return;
        }
        _httpRequest->addHeader(line);
        bufferStr->erase(0, pos + 2);
        pos = bufferStr->find("\r\n");
    }
    this->_buffer = *bufferStr;
}

void HttpRequestParser::handleBodyState(std::string *bufferStr) {
    std::string line;
    if (bufferStr->empty())  
        return;
    this->_buffer = *bufferStr;
    _readBodySize += bufferStr->size();
}