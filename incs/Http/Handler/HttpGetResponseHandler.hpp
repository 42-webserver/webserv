#ifndef HTTPGETRESPONSEHANDLER_HPP
#define HTTPGETRESPONSEHANDLER_HPP

#include "../Exception/BadRequestException.hpp"
#include "../Exception/NotFoundException.hpp"
#include "HttpRequestHandler.hpp"
#include "../Handler/ErrorPageHandler.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Utils/RouterUtils.hpp"
#include "../Utils/FileUploader.hpp"
#include "../../Cache/cache.hpp"
#include <map>

class HttpGetResponseHandler : public HttpRequestHandler
{
private:
static std::map<std::string, std::string> _contentTypeMap;
    
static std::map<std::string, std::string> _initializeContentTypeMap()
{
    std::map<std::string, std::string> map;
    map[".html"] = "text/html";
    map[".txt"] = "text/plain";
    map[".css"] = "text/css";
    map[".js"] = "text/javascript";
    map[".xml"] = "text/xml";
    map[".jpg"] = "image/jpeg";
    map[".jpeg"] = "image/jpeg";
    map[".png"] = "image/png";
    map[".gif"] = "image/gif";
    map[".bmp"] = "image/bmp";
    map[".zip"] = "application/zip";
    map[".pdf"] = "application/pdf";
    map[".json"] = "application/json";
    return map;
}


public:
    HttpGetResponseHandler();
    ~HttpGetResponseHandler();
    ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm) \
        throw (HttpException);

private:
    void _makeResponseHeader(std::string &path, ft::shared_ptr<HttpResponse> response, ft::shared_ptr<HttpRequest> request);
    std::string _getContentType(const std::string& fileType);

};

#endif