#include "../../../incs/Http/Handler/HttpCgiResponseHandler.hpp"

HttpCgiResponseHandler::HttpCgiResponseHandler()
{
}

HttpCgiResponseHandler::~HttpCgiResponseHandler()
{
}

ft::shared_ptr<HttpResponse> HttpCgiResponseHandler::handleRequest(ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm)
{
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
        executeCgi(req, pipefd, cgiPath);
    }
    else {
        close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            while(true) {
                char buf[1024];
                ssize_t bytes = read(pipefd[0], buf, sizeof(buf));
                if (bytes == -1)
                    throw std::runtime_error("Failed to read from pipe.");
                if (bytes == 0)
                    break;
                buffer.insert(buffer.end(), buf, buf + bytes);
                makeResponseHeader(buffer, response);
            }
        }
        else
            throw std::runtime_error("CGI script exited with an error.");
    }
    response->setBody(buffer);
    return response;
}

void HttpCgiResponseHandler::makeResponseHeader(std::string &buffer, ft::shared_ptr<HttpResponse> response) throw (CgiResponseNotValidException){
    std::string::size_type pos = buffer.find("\n\n");
    while (pos != std::string::npos){
        std::string header = buffer.substr(0, pos);
        std::string key = header.substr(0, header.find(":"));
        std::string value = header.substr(header.find(":") + 1);
        response->addHeader(key, value);
        buffer.erase(0, pos + 2);
        pos = buffer.find("\n\n");
    }
}


void HttpCgiResponseHandler::executeCgi(ft::shared_ptr<HttpRequest> request, int pipefd[2], std::string &cgiPath){
    char* const scriptPath = const_cast<char*>(cgiPath.c_str());
    char* argv[] = { "/usr/bin/python3", scriptPath, NULL};

    close(pipefd[0]);
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        throw std::runtime_error("Failed to duplicate file descriptor.");
    close(pipefd[1]);

    // 환경 변수 설정
    std::map<std::string, std::string> envMap;
    envMap["ENV_VARIABLE_1"] = "Value1";
    envMap["ENV_VARIABLE_2"] = "Value2";

    // 환경 변수 정보를 저장할 문자열 배열
    char* envp[envMap.size() + 1]; 

    populateEnvp(envMap, envp);

    // CGI 스크립트 실행
    if (execve("/usr/bin/python3", argv, envp) == -1)
        throw std::runtime_error("Failed to execute CGI script.");
}

void HttpCgiResponseHandler::populateEnvp(const std::map<std::string, std::string>& envMap, char* envp[]) {
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator it = envMap.begin(); it != envMap.end(); ++it) {
        std::string envVar = it->first + "=" + it->second;
        envp[i] = new char[envVar.size() + 1];
        std::strcpy(envp[i], envVar.c_str());
        ++i;
    }
    envp[i] = NULL;
}