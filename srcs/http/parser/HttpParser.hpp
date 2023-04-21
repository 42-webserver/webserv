#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <string>
#include "../request/HttpRequest.hpp"
#include "../response/HttpResponse.hpp"

class HttpParser
{

public:
    HttpParser();
    ~HttpParser();

    HttpRequest &parseRequest(std::string &request);
    std::string &parseResponse(HttpResponse &response);
};

#endif