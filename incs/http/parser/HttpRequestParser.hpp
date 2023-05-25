#ifndef HTTP_REQUEST_PARSER_HPP
#define HTTP_REQUEST_PARSER_HPP

#include "../request/HttpRequest.hpp"
#include <vector> 

typedef enum State{
        BEFORE,
        START_LINE,
        HEADERS,
        BODY
} RequestParseState;

class HttpRequestParser{

private:
    HttpRequest         *_httpRequest;
    RequestParseState   _state;
    std::string         _buffer;
    int                 _readBodySize;

public:
    HttpRequestParser(HttpRequest *httpRequest);
    void parseRequest(const std::vector<char> &buffer);
    void clearBuffer();
    RequestParseState getState();
    int getReadBodySize();
    HttpRequest *getHttpRequest();
    std::string getBuffer();

private:
    void handleStartLineState(std::string &bufferStr);
    void handleHeaderState(std::string &bufferStr);
    void handleBodyState(std::string &bufferStr);
};

#endif