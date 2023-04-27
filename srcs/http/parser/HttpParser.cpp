#include "HttpParser.hpp"


HttpParser::HttpParser()
{
}

HttpRequest * HttpParser::parseRequest(std::string *request){
    HttpRequest *req = new HttpRequest();
    std::string line;
    std::string::size_type pos;
    std::string * body;

    pos = request->find("\r\n");
    line = request->substr(0, pos);
    request->erase(0, pos + 2);
    req->setStartLine(line);
    while (request->find("\r\n") != std::string::npos)
    {
        pos = request->find("\r\n");
        if(pos == 0)
        {
            request->erase(0, 2);
            break;
        }
        line = request->substr(0, pos);
        req->addHeader(line);
        request->erase(0, pos + 2);
    }
    body = new std::string(*request);
    req->setBody(body);
    return (req);
}

std::string *HttpParser::parseResponse(HttpResponse *response){
    std::string *str = new std::string();
    std::string key, value;
    std::multimap<std::string, std::string>::const_iterator it; 
    std::stringstream ss;
    std::vector<Cookie> cookies = response->getCookies();
    std::vector<Cookie>::iterator itCookie;

    ss << response->getStatusCode();
    *str += response->getProtocol();
    *str += "/";
    *str += response->getVersion();
    *str += " ";
    *str += ss.str();
    *str += " ";
    *str += response->getReasonPhrase();
    *str += "\r\n";
    it = response->getHeaders().begin();
    while (it != response->getHeaders().end())
    {
        key = it->first;
        value = it->second;
        *str += key;
        *str += ": ";
        *str += value;
        *str += "\r\n";
        it++;
    }
    itCookie = cookies.begin();
    while (itCookie != cookies.end())
    {
        *str += "Set-Cookie: ";
        *str += itCookie->getKey();
        *str += "=";
        *str += itCookie->getValue();
        *str += "; Max-Age=";
        *str += itCookie->getMaxAge();
        *str += "\r\n";
        itCookie++;
    }

    *str += "\r\n";
    *str += *response->getBody();
    return (str);
}