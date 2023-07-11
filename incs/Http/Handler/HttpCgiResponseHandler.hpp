#ifndef HTTPCGIRESPONSEHANDLER_HPP
#define HTTPCGIRESPONSEHANDLER_HPP

#include "HttpRequestHandler.hpp"
#include <unistd.h>
#include "../../Server/VirtualServerManager.hpp"
#include "../Exception/ServerErrorException.hpp"
#include "../Utils/RouterUtils.hpp"

class HttpCgiResponseHandler : public HttpRequestHandler
{
public:
    class CgiResponseNotValidException : public ServerErrorException {
    private:
        const char *msg;
    public:
        CgiResponseNotValidException(const char *msg);
        virtual const char* what() const throw();
    };

public:
    HttpCgiResponseHandler();
    ~HttpCgiResponseHandler();
    ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm) throw (ServerErrorException);

private:
    void executeCgi(ft::shared_ptr<HttpRequest> request, int pipefd[2], std::string &cgiPath);
    void populateEnvp(const std::map<std::string, std::string>& envMap, char* envp[]);
    void makeResponseHeader(std::string &buffer, ft::shared_ptr<HttpResponse> response) throw (CgiResponseNotValidException);
};


#endif