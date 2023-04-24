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
    ~HttpResponse();
    void setBody(const std::string & body);
    void addHeader(const std::string & key, const std::string & value);
    void setStatusCode(HttpStatusCode code);
    const std::string &getVersion();
    const std::string &getBody();
    const std::string &getProtocol();
    const std::string &getReasonPhrase();
    int getStatusCode();
    const std::multimap<std::string, std::string> &getHeaders() const;
};

#endif
