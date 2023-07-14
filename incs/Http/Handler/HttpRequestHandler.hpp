#ifndef HTTPREQUESTHANDLER_HPP
#define HTTPREQUESTHANDLER_HPP

#include "../Response/HttpResponse.hpp"
#include "../Request/HttpRequest.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "../../Server/VirtualServerManager.hpp"

class HttpRequestHandler
{
public:
    virtual ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm) = 0;
};

#endif