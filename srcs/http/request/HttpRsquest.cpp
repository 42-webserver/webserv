#include "HttpRequest.hpp"


void HttpRequest::setBody(std::string body)
{
    _body = body;
}

void HttpRequest::setHost(std::string host)
{
    _host = host;
}

void HttpRequest::addHeader(std::string header)
{
    std::string::size_type pos;
    std::string key;
    std::string value;

    pos = header.find(": ");
    key = header.substr(0, pos);
    value = header.substr(pos + 2, header.size());
    this->_headers.insert(std::pair<std::string, std::string>(key, value));
    if (key == "Cookie"){
        std::string::size_type pos;
        std::string cookieKey;
        std::string cookieValue;
        std::string tmp;

        while (value.find("; ") != std::string::npos)
        {
            pos = value.find("; ");
            tmp = value.substr(0, pos);
            value.erase(0, pos + 2);
            cookieKey = tmp.substr(0, tmp.find("="));
            cookieValue = tmp.substr(tmp.find("=") + 1, tmp.size());
            this->_cookies.insert(std::pair<std::string, std::string>(cookieKey, cookieValue));
        }
    }
}

void HttpRequest::setStartLine(std::string line)
{
    std::string::size_type pos;

    pos = line.find(" ");
    _method = line.substr(0, pos);
    line.erase(0, pos + 1);
    pos = line.find(" ");
    _uri = line.substr(0, pos);
    line.erase(0, pos + 1);
    _version = line.substr(0, line.find("\r\n"));
    line.erase(0, line.find("\r\n") + 2);
}
