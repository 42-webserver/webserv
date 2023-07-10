#ifndef HTTPCGIRESPONSEHANDLER_HPP
#define HTTPCGIRESPONSEHANDLER_HPP

#include "HttpRequestHandler.hpp"
#include <unistd.h>
#include "../../Server/VirtualServer.hpp"
#include "../../Server/VirtualServerManager.hpp"

class HttpCgiResponseHandler : public HttpRequestHandler
{
public:
    HttpCgiResponseHandler();
    ~HttpCgiResponseHandler();
    ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm);

private:
    void executeCgi(ft::shared_ptr<VirtualServer> targetServer, ft::shared_ptr<HttpRequest> request, int pipefd[2]);
};


#endif