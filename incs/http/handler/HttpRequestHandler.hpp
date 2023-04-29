#ifndef HTTP_REQUEST_HANDLER_HPP
#define HTTP_REQUEST_HANDLER_HPP

#include "../request/HttpRequest.hpp"
#include "../response/HttpResponse.hpp"
#include "./ErrorPageHandler.hpp"
#include "../../../incs/server/Server.hpp"
#include "../utils/HttpMethod.hpp"
#include <sstream>

class HttpRequestHandler
{
private:

public:
    HttpRequestHandler();
    HttpResponse * handle(HttpRequest *request, Server *server);

private:
    HttpResponse * handleGet(HttpRequest *request, Server *server);
    bool isAllowdMethod(std::string method);
    HttpResponse * methodNotAllowed();
    std::string * getRequestPage(HttpRequest *request, Server *server);

public:
    class PageNotFoundException : public std::exception
    {
    private:
    const char *_message;
    public:
        PageNotFoundException();
        PageNotFoundException(std::string msg);
        virtual const char *what() const throw();
    };
};

#endif