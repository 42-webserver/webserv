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
    std::string *_body;
    std::string _protocol;

    std::multimap<std::string, std::string> _headers;
    std::map<std::string, std::string> _queries;
    std::map<std::string, std::string> _cookies;
    

public:
    HttpRequest();
    ~HttpRequest();
    void setBody( std::string * body);
    void addHeader(const std::string & header);
    void setStartLine(std::string line);
    std::string getMethod();
    std::string getUri();
    std::string getVersion();
    std::string *getBody();
    std::string getProtocol();
    std::multimap<std::string, std::string> getHeaders();
    std::map<std::string, std::string> getQueries();
    std::map<std::string, std::string> getCookies();

private:
    void setQuery(std::string & uri);
    void setCookie(std::string & cookie);
    bool esacpeComma(std::string & value);
    void handleMultipleValueHeader(std::string & value, std::string & key);
};

#endif