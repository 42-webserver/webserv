#include "../../../incs/Http/Handler/HttpGetResponseHandler.hpp"

HttpGetResponseHandler::HttpGetResponseHandler()
{
    if (_contentTypeMap.empty())
        _initializeContentTypeMap();
}

HttpGetResponseHandler::~HttpGetResponseHandler()
{
}

ft::shared_ptr<HttpResponse> HttpGetResponseHandler::handleRequest(ft::shared_ptr<HttpRequest> req, 
        ft::shared_ptr<VirtualServerManager> vsm) throw (BadRequestException){
    ft::shared_ptr<HttpResponse> response(new HttpResponse());
    std::string path = RouterUtils::findPriorityPathWithIndex(vsm, req);
    // get body

}

void HttpGetResponseHandler::_makeResponseHeader(std::string &path, ft::shared_ptr<HttpResponse> response, ft::shared_ptr<HttpRequest> request){
    std::string fileType =path.substr(path.find_last_of(".") + 1);
    response->addHeader("Content-Type", _getContentType(fileType));
    if (request->getVersion() =="1.1")
        response->addHeader("Connection", "keep-alive");
    

}

std::string HttpGetResponseHandler::_getContentType(const std::string& fileType) {
    std::map<std::string, std::string>::const_iterator it = _contentTypeMap.find(fileType);
    if (it != _contentTypeMap.end()) {
        return it->second;
    }
    // 매핑이 없는 경우 기본값으로 "application/octet-stream"을 반환
    return "application/octet-stream";
}

void HttpGetResponseHandler::_initializeContentTypeMap(){
    _contentTypeMap[".html"] = "text/html";
    _contentTypeMap[".txt"] = "text/plain";
    _contentTypeMap[".css"] = "text/css";
    _contentTypeMap[".js"] = "text/javascript";
    _contentTypeMap[".xml"] = "text/xml";
    _contentTypeMap[".jpg"] = "image/jpeg";
    _contentTypeMap[".jpeg"] = "image/jpeg";
    _contentTypeMap[".png"] = "image/png";
    _contentTypeMap[".gif"] = "image/gif";
    _contentTypeMap[".bmp"] = "image/bmp";
    _contentTypeMap[".zip"] = "application/zip";
    _contentTypeMap[".pdf"] = "application/pdf";
    _contentTypeMap[".json"] = "application/json";
}