#include "../../../incs/Http/Handler/HttpGetResponseHandler.hpp"

//fix daegulee: may be this logic in Write Event.

std::map<std::string, std::string> HttpGetResponseHandler::_contentTypeMap = HttpGetResponseHandler::_initializeContentTypeMap();

HttpGetResponseHandler::HttpGetResponseHandler()
{
}

HttpGetResponseHandler::~HttpGetResponseHandler()
{
}

ft::shared_ptr<HttpResponse> HttpGetResponseHandler::handleRequest(ft::shared_ptr<HttpRequest> req, 
        ft::shared_ptr<VirtualServerManager> vsm) throw (HttpException){
    ft::shared_ptr<HttpResponse> response = ft::make_shared<HttpResponse>();
    std::string path = RouterUtils::findPriorityPathWithIndex(vsm, req);

    try{
        Cache &cache = Cache::getInstance();
        std::vector<char> content = cache.getFileContent(path);
        response->setBody(content);
    } catch (Cache::BigFileException &e){
        response->setFileName(path);
    }
    _makeResponseHeader(path, response, req);
    return response;
}

void HttpGetResponseHandler::_makeResponseHeader(std::string &path, ft::shared_ptr<HttpResponse> response, ft::shared_ptr<HttpRequest> request){
    std::string fileType =path.substr(path.find_last_of(".") + 1);
    response->addHeader("Content-Type", _getContentType(fileType));
    if (request->getVersion() =="1.1" || (request->getVersion() =="1.0" && request->getHeader("Connection") =="keep-alive"))
        response->addHeader("Connection", "keep-alive");
    long long contentLengthTmp;
    if (response->getFileName().empty())
        contentLengthTmp = response->getBody().size();
    else
        contentLengthTmp = FileUploader::getFileSize(response->getFileName());
    std::stringstream ss;
    ss << contentLengthTmp;
    std::string contentLength = ss.str();
    response->addHeader("Content-Length", contentLength);
}

std::string HttpGetResponseHandler::_getContentType(const std::string& fileType) {
    std::map<std::string, std::string>::const_iterator it = _contentTypeMap.find(fileType);
    if (it != _contentTypeMap.end()) {
        return it->second;
    }
    // 매핑이 없는 경우 기본값으로 "application/octet-stream"을 반환
    return "application/octet-stream";
}

