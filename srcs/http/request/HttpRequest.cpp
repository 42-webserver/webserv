#include "HttpRequest.hpp"


void HttpRequest::setBody(const std::string & body)
{
    _body = body;
}

void HttpRequest::addHeader(const std::string & header)
{
    std::string::size_type pos;
    std::string key;
    std::string value;

    pos = header.find(": ");
    key = header.substr(0, pos);
    value = header.substr(pos + 2);
    handleMultipleValueHeader(value, key);
    if (key == "Cookie")
        setCookie(value);
}

void HttpRequest::handleMultipleValueHeader(std::string & value, std::string & key)
{
    std::string::size_type pos;

    while (value.find(", ") != std::string::npos)
    {
        pos = value.find(", ");
        this->_headers.insert(std::pair<std::string, std::string>(key, value.substr(0, pos)));
        value.erase(0, pos + 2);
    }
    this->_headers.insert(std::pair<std::string, std::string>(key, value));
}

void HttpRequest::setCookie(std::string & cookie)
{
    std::string::size_type pos;
    std::string key, value, tmp;

    while (cookie.find("; ") != std::string::npos)
    {
        pos = cookie.find("; ");
        tmp = cookie.substr(0, pos);
        cookie.erase(0, pos + 2);
        key = tmp.substr(0, tmp.find("="));
        value = tmp.substr(tmp.find("=") + 1);
        this->_cookies.insert(std::pair<std::string, std::string>(key, value));
    }
    key = cookie.substr(0, cookie.find("="));
    value = cookie.substr(cookie.find("=") + 1);
    this->_cookies.insert(std::pair<std::string, std::string>(key, value));
}

void HttpRequest::setStartLine(std::string line)
{
    std::string::size_type pos;

    pos = line.find(" ");
    this->_method = line.substr(0, pos);
    line.erase(0, pos + 1);
    pos = line.find(" ");
    this->_uri = line.substr(0, pos);
    if (this->_uri.find("?") != std::string::npos)
        setQuery(this->_uri);
    line.erase(0, pos + 1);
    this->_version = line.substr(0, line.find("\r\n"));
    line.erase(0, line.find("\r\n") + 2);
}

void HttpRequest::setQuery(std::string & uri)
{
    std::string::size_type pos;
    std::string key, value, query, tmp;

    query = uri.substr(uri.find("?") + 1);
    while (query.find("&") != std::string::npos){
        pos = query.find("&");
        tmp = query.substr(0, pos);
        query.erase(0, pos + 1);
        value = tmp.substr(tmp.find("=") + 1);
        key = tmp.substr(0, tmp.find("="));
        this->_queries.insert(std::pair<std::string, std::string>(key, value));
    }
    key = query.substr(0, query.find("="));
    value = query.substr(query.find("=") + 1);
    this->_queries.insert(std::pair<std::string, std::string>(key, value));
}
