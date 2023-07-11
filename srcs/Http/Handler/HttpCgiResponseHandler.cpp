#include "../../../incs/Http/Handler/HttpCgiResponseHandler.hpp"

HttpCgiResponseHandler::HttpCgiResponseHandler(): _isHeaderState(true), _contentLength(INT_MAX)
{
}

HttpCgiResponseHandler::~HttpCgiResponseHandler()
{
}

ft::shared_ptr<HttpResponse> HttpCgiResponseHandler::handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm){
    ft::shared_ptr<HttpResponse> response(new HttpResponse());
    std::string cgiPath = RouterUtils::findPath(vsm, req);
    int pipefd[2];
    std::string buffer;
    if (pipe(pipefd) == -1)
        throw std::runtime_error("Failed to create pipe.");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("Failed to create pipe.");

    if (pid == 0){
        _executeCgi(req, pipefd, cgiPath);
    }
    else {
        close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            ssize_t bytes;
            char buf[1024];
            while(bytes = read(pipefd[0], buf, sizeof(buf)) > 0) {
                buffer.insert(buffer.end(), buf, buf + bytes);
                if (_isHeaderState)
                    _makeResponseHeader(buffer, response);
                else{
                    _makeResponseBody(buffer, response);
                    if (response->getBody().size() == _contentLength)
                        break;
                }
            }
        }
        else
            throw std::runtime_error("CGI script exited with an error.");
    }
    _addTypeAndLength(response);
    return response;
}

void HttpCgiResponseHandler::_addTypeAndLength(ft::shared_ptr<HttpResponse> response){
    std::multimap<std::string, std::string> headers = response->getHeaders();
    if (headers.empty())
        throw CgiResponseNotValidException("CGI response is not valid (No Header).");
    
    std::multimap<std::string, std::string>::iterator it = headers.find("Content-Type");
    if (it == headers.end())
        response->addHeader("Content-Type", "text/plain");

    it = headers.find("Content-Length");
    if (it == headers.end())
        response->addHeader("Content-Length", std::to_string(response->getBody().size()));
}

void HttpCgiResponseHandler::_makeResponseBody(std::string &buffer, ft::shared_ptr<HttpResponse> response){
    if (response->getBody().size() + buffer.size() > _contentLength){
        std::string tmp = buffer.substr(0, _contentLength - response->getBody().size());
        response->setBody(tmp);
    }else{
        response->setBody(buffer);
    }
}

void HttpCgiResponseHandler::_makeResponseHeader(std::string &buffer, ft::shared_ptr<HttpResponse> response) throw (CgiResponseNotValidException){
    std::string::size_type pos = buffer.find("\n");
    while (pos != std::string::npos){
        std::string header = buffer.substr(0, pos);
        if (header.empty()){
            _isHeaderState = false;
            break;
        }
        std::string key = header.substr(0, header.find(":"));
        std::string value = header.substr(header.find(":") + 1);
        if (key == "Content-Length")
            _contentLength = std::atoi(value.c_str());
        response->addHeader(key, value);
        buffer.erase(0, pos + 2);
        pos = buffer.find("\n");
    }
}


void HttpCgiResponseHandler::_executeCgi(ft::shared_ptr<HttpRequest> request, int pipefd[2], std::string &cgiPath){
    char* const scriptPath = const_cast<char*>(cgiPath.c_str());
    char* argv[] = { "/usr/bin/python3", scriptPath, NULL};

    close(pipefd[0]);
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        throw std::runtime_error("Failed to duplicate file descriptor.");
    close(pipefd[1]);

    // 환경 변수 설정
    std::map<std::string, std::string> envMap;
    _fillMapWithQuery(envMap, request);

    // 환경 변수 정보를 저장할 문자열 배열
    char* envp[envMap.size() + 1]; 

    _populateEnvp(envMap, envp);

    // CGI 스크립트 실행
    if (execve("/usr/bin/python3", argv, envp) == -1)
        throw FailExecuteCgiException("Failed to execute CGI script.");
}

void HttpCgiResponseHandler::_fillMapWithQuery(std::map<std::string, std::string>& envMap, ft::shared_ptr<HttpRequest> request){
    std::map<std::string, std::string> queryMap = request->getQueries();
    for (std::map<std::string, std::string>::iterator it = queryMap.begin(); it != queryMap.end(); ++it) {
        envMap[it->first] = it->second;
    }
}

void HttpCgiResponseHandler::_populateEnvp(const std::map<std::string, std::string>& envMap, char* envp[]) {
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator it = envMap.begin(); it != envMap.end(); ++it) {
        std::string envVar = it->first + "=" + it->second;
        envp[i] = new char[envVar.size() + 1];
        std::strcpy(envp[i], envVar.c_str());
        ++i;
    }
    envp[i] = NULL;
}

HttpCgiResponseHandler::CgiResponseNotValidException::CgiResponseNotValidException(const char *msg) : msg(msg) {}
const char* HttpCgiResponseHandler::CgiResponseNotValidException::what() const throw() { return msg; }

HttpCgiResponseHandler::FailExecuteCgiException::FailExecuteCgiException(const char *msg) : msg(msg) {}
const char* HttpCgiResponseHandler::FailExecuteCgiException::what() const throw() { return msg; }