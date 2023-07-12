#ifndef HTTPGETRESPONSEHANDLER_HPP
#define HTTPGETRESPONSEHANDLER_HPP

#include "../Exception/BadRequestException.hpp"
#include "../Exception/NotFoundException.hpp"
#include "HttpRequestHandler.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Utils/RouterUtils.hpp"
#include "../Utils/FileUploader.hpp"
#include <map>

class HttpGetResponseHandler : public HttpRequestHandler
{
private:
    static std::map<std::string, std::string> _contentTypeMap;
public:
    HttpGetResponseHandler();
    ~HttpGetResponseHandler();
    ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm) \
        throw (NotFoundException);

private:
    void _makeResponseHeader(std::string &path, ft::shared_ptr<HttpResponse> response, ft::shared_ptr<HttpRequest> request);
    std::string _getContentType(const std::string& fileType);
    static void _initializeContentTypeMap();
    
};

#endif