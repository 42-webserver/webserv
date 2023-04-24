#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>
#include "../utils/HttpStatus.hpp"

class HttpResponse
{
private:
    std::string _version;
    std::string _body;
    std::string _protocol;
    HttpStatusCode _statusCode;
    
    std::multimap<std::string, std::string> _headers;

public:
    HttpResponse();
    void setBody(const std::string & body);
    void addHeader(const std::string & key, const std::string & value);
    void setStatusCode(HttpStatusCode code);
    std::string getVersion();
    std::string getBody();
    std::string getProtocol();
    std::string getReasonPhrase();
    int getStatusCode();
    const std::multimap<std::string, std::string> &getHeaders() const;
};

#endif
