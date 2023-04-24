#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>

class HttpRequest
{
private:
    std::string _method;
    std::string _uri;
    std::string _version;
    std::string _body;
    std::string _protocol;

    std::multimap<std::string, std::string> _headers;
    std::map<std::string, std::string> _queries;
    std::map<std::string, std::string> _cookies;
    

public:
    HttpRequest();
    void setBody(const std::string & body);
    void addHeader(const std::string & header);
    void setStartLine(std::string line);

private:
    void setQuery(std::string & uri);
    void setCookie(std::string & cookie);
    void handleMultipleValueHeader(std::string & value, std::string & key);
};

#endif