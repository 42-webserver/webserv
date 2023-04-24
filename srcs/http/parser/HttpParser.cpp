#include "HttpParser.hpp"

HttpRequest & HttpParser::parseRequest(std::string &request){
    HttpRequest req;
    std::string line;
    std::string::size_type pos;

    pos = request.find("\r\n");
    line = request.substr(0, pos);
    request.erase(0, pos + 2);
    req.setStartLine(line);
    while (request.find("\r\n") != std::string::npos)
    {
        pos = request.find("\r\n");
        if(pos == 0)
        {
            request.erase(0, 2);
            break;
        }
        line = request.substr(0, pos);
        req.addHeader(line);
        request.erase(0, pos + 2);
    }
    req.setBody(request);
    return (req);
}

std::string &HttpParser::parseResponse(HttpResponse &response){
    std::string str;
    std::string key;
    std::string value;
    std::multimap<std::string, std::string>::const_iterator it; 

    str += response.getVersion();
    str += " ";
    str += response.getStatusCode();
    str += " ";
    str += response.getReasonPhrase();
    str += "\r\n";
    it = response.getHeaders().begin();
    while (it != response.getHeaders().end())
    {
        key = it->first;
        value = it->second;
        str += key;
        str += ": ";
        str += value;
        str += "\r\n";
        it++;
    }
    str += "\r\n";
    str += response.getBody();
    return (str);
}