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
    int pipefd[2];
    std::string buffer;
    if (pipe(pipefd) == -1)
        throw std::runtime_error("Failed to create pipe.");

    pid_t pid = fork();
    if (pid == -1)
        throw std::runtime_error("Failed to create pipe.");

    if (pid == 0){
        // executeCgi(targetServer, request, pipefd);
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
            }
        }
        else
            throw std::runtime_error("CGI script exited with an error.");
    }
    response->setBody(buffer);
    return response;
}


void HttpCgiResponseHandler::executeCgi(ft::shared_ptr<VirtualServer> targetServer, ft::shared_ptr<HttpRequest> request, int pipefd[2]){
    char* const scriptPath = "upload.py";
    char* const argv[] = { "python3", scriptPath, "content data", NULL};

    close(pipefd[0]);
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        throw std::runtime_error("Failed to duplicate file descriptor.");
    close(pipefd[1]);
    // 환경 변수 설정
    char* const envp[] = { NULL };

    // CGI 스크립트 실행
    if (execve("/usr/bin/python3", argv, envp) == -1)
        throw std::runtime_error("Failed to execute CGI script.");
}