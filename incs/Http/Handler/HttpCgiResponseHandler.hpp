#ifndef HTTPCGIRESPONSEHANDLER_HPP
#define HTTPCGIRESPONSEHANDLER_HPP

#include "HttpRequestHandler.hpp"
#include <unistd.h>
#include "../../Server/VirtualServerManager.hpp"
#include "../Exception/ServerErrorException.hpp"
#include "../Utils/RouterUtils.hpp"
#include <sstream>

class HttpCgiResponseHandler : public HttpRequestHandler
{
private:
    bool    _isHeaderState;
    int     _contentLength;

public:
    class CgiResponseNotValidException : public ServerErrorException {
        public:
            CgiResponseNotValidException() : ServerErrorException(INTERNAL_SERVER_ERROR) {}
            virtual const char* what() const throw(){
                return "Cgi response is not valid";
            }
    };

    class FailExecuteCgiException : public ServerErrorException {
        public:
            FailExecuteCgiException() : ServerErrorException(INTERNAL_SERVER_ERROR) {}
            virtual const char* what() const throw(){
                return "Fail to execute cgi";
            }
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
    void _makeResponseBody(std::string &buffer, ft::shared_ptr<HttpResponse> response);
    void _addTypeAndLength(ft::shared_ptr<HttpResponse> response);
};


#endif