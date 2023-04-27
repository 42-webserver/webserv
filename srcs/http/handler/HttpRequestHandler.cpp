#include "HttpRequestHandler.hpp"

HttpRequestHandler::HttpRequestHandler(){}

HttpResponse * HttpRequestHandler::handle(HttpRequest *request, Server *server)
{
    if(isAllowdMethod(request->getMethod()) == false)
        return methodNotAllowed();
    else if (request->getMethod() == HTTP_METHOD_GET)
        return handleGet(request, server);
    else
        return methodNotAllowed();
}

HttpResponse * HttpRequestHandler::handleGet(HttpRequest *request,  Server *server)
{
    HttpResponse *response = new HttpResponse();
    std::string *findedRequestPage;
    std::stringstream ss;
    try{
        findedRequestPage = getRequestPage(request, server);
        response->setStatusCode(OK);
        response->addHeader("Content-Type", "text/html");
        ss << (*findedRequestPage).length();
        response->addHeader("Content-Length", ss.str());
        response->setBody(findedRequestPage);
        return response;
    } catch (PageNotFoundException &e) {
        delete response;
        response = new HttpResponse();
        std::string *pageNotFoundHtml = ErrorPageHandler::getErrorPage(NOT_FOUND);
        response->setStatusCode(NOT_FOUND);
        response->addHeader("Content-Type", "text/html");
        ss << (*pageNotFoundHtml).length();
        response->addHeader("Content-Length", ss.str());
        response->setBody(pageNotFoundHtml);
        return response;
    }
}

HttpResponse * HttpRequestHandler::methodNotAllowed()
{
    HttpResponse *response = new HttpResponse();
    std::string *methodNotAllowdHtml = ErrorPageHandler::getErrorPage(METHOD_NOT_ALLOWED);
    std::stringstream ss;
    response->setStatusCode(METHOD_NOT_ALLOWED);
    response->addHeader("Content-Type", "text/html");
    ss << (*methodNotAllowdHtml).length();
    response->addHeader("Content-Length", ss.str());
    response->setBody(methodNotAllowdHtml);
    return response;
}

std::string * HttpRequestHandler::getRequestPage(HttpRequest *request, Server *server)
{
    std::string requestPage = request->getUri();
    std::string *findedRequestPage = new std::string();
    std::string root = server->getRoot();
    std::vector<std::string> index = server->getIndexes();
    std::vector<std::string> defaultPages;
    std::ifstream ifs;

    if (requestPage == "/"){
        for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++)
            defaultPages.push_back(root + "/" + *it);
        for (std::vector<std::string>::iterator it = defaultPages.begin(); it != defaultPages.end(); it++){
            ifs.open(it->c_str());
            if (ifs.is_open()) {
                while (!ifs.eof()) {
                    std::string str;
                    std::getline(ifs, str);
                    findedRequestPage->append(str);
                }
                ifs.close();
                return findedRequestPage;
            }
        }
    }
    else{
        requestPage = root + requestPage;
        ifs.open(requestPage.c_str());
        if (ifs.is_open()) {
            while (!ifs.eof()) {
                std::string str;
                std::getline(ifs, str);
                findedRequestPage->append(str);
            }
            ifs.close();
            return findedRequestPage;
        }
    }
    delete findedRequestPage;
    throw PageNotFoundException();
}

bool HttpRequestHandler::isAllowdMethod(std::string method)
{
    if (method == HTTP_METHOD_GET)
        return true;
    return false;
 /*
    Todo: check if method is allowed
 */
}

HttpRequestHandler::PageNotFoundException::PageNotFoundException(){
    _message = "Page not found";
}

HttpRequestHandler::PageNotFoundException::PageNotFoundException(std::string msg){
    _message = msg.c_str();
}

const char *HttpRequestHandler::PageNotFoundException::what() const throw(){
    return _message;
}