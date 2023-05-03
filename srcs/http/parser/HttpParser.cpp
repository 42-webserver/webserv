#include "../../../incs/http/parser/HttpParser.hpp"


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
    std::string *str;
    std::string key, value;
    std::multimap<std::string, std::string>::const_iterator it; 
    std::stringstream ss, ss2;
    std::vector<Cookie> cookies = response->getCookies();
    std::vector<Cookie>::iterator itCookie;

    ss << response->getStatusCode();
    ss2 << response->getProtocol() << "/" << response->getVersion() << " " << ss.str() << " " << response->getReasonPhrase() << "\r\n";
    it = response->getHeaders().begin();
    while (it != response->getHeaders().end())
    {
        key = it->first;
        value = it->second;
        ss2 << key << ": " << value << "\r\n";
        it++;
    }
    itCookie = cookies.begin();
    while (itCookie != cookies.end())
    {
        ss2 << "Set-Cookie: " << itCookie->getKey() << "=" << 
            itCookie->getValue() << "; Max-Age=" << itCookie->getMaxAge() << "\r\n";
        itCookie++;
    }
    ss2 << "\r\n";
    ss2 << *response->getBody();
    str = new std::string(ss2.str());
    return (str);
}