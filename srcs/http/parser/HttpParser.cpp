#include "HttpParser.hpp"

HttpRequest & HttpParser::parseRequest(std::string &request){

    HttpRequest req;
    std::string line;
    std::string::size_type pos;

    pos = request.find("\r\n");
    line = request.substr(0, pos);
    request.erase(0, pos + 2);
    req.setStartLine(line);
    while (line.find("\r\n") != std::string::npos)
    {
        pos = line.find("\r\n");
        req.addHeader(line.substr(0, pos));
        line.erase(0, pos + 2);
    }
    req.setBody(line);
    return (req);
}
