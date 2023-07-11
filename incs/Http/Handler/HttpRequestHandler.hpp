#ifndef HTTPREQUESTHANDLER_HPP
#define HTTPREQUESTHANDLER_HPP

#include "../Response/HttpResponse.hpp"
#include "../Request/HttpRequest.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"

class HttpRequestHandler
{
public:
    virtual ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req) = 0;
};

#endif