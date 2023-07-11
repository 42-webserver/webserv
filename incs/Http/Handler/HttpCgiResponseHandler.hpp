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

    class FailExecuteCgiException : public ServerErrorException {
         private:
            const char *msg;
        public:
            FailExecuteCgiException(const char *msg);
            virtual const char* what() const throw();
    };

public:
    HttpCgiResponseHandler();
    ~HttpCgiResponseHandler();
    ft::shared_ptr<HttpResponse> handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm) throw (ServerErrorException);

private:
    void _executeCgi(ft::shared_ptr<HttpRequest> request, int pipefd[2], std::string &cgiPath);
    void _populateEnvp(const std::map<std::string, std::string>& envMap, char* envp[]);
    void _makeResponseHeader(std::string &buffer, ft::shared_ptr<HttpResponse> response) throw (CgiResponseNotValidException);
    void _fillMapWithQuery(std::map<std::string, std::string> &envMap, ft::shared_ptr<HttpRequest> request);
};


#endif