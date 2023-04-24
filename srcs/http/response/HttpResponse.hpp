#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>

class HttpResponse
{
private:
    std::string _version;
    std::string _body;
    std::string _protocol;
    int         _statusCode;
    
    std::multimap<std::string, std::string> _headers;
    std::map<std::string, std::string> _cookies;
};

#endif
