#include <Http/Response/HttpResponseBuilder.hpp>

ContentLength::ContentLength(int contentLength) : _transferEncodingHeader(""), _contentLength(contentLength), _contentLengthHeaderType(e_content_length_header)
{
}
ContentLength::ContentLength(void) : _transferEncodingHeader("Transfer-Encoding: chunked"), _contentLength(-1), _contentLengthHeaderType(e_chunked_header)
{
}

std::string ContentLength::getContentLengthHeader(void)
{
    if (this->_contentLengthHeaderType == e_content_length_header) {
        std::ostringstream oss;
        oss << "Content-Length: " << this->_contentLength;
        return (oss.str());
    }
    else
        return (this->_transferEncodingHeader);
}

DirectoryException::DirectoryException() {
}

DirectoryException::~DirectoryException() throw() {
}

const char *DirectoryException::what() const throw() {
    return "DirectoryException";
}

std::string HttpResponseBuilder::_makeContentTypeHeader(ft::shared_ptr<HttpRequest> request) {
    std::string uri = request->getUri();

    if (uri.find_last_of(".") == std::string::npos) {
        throw DirectoryException();
    }

    std::string extension = uri.substr(uri.find_last_of(".") + 1);
    for (std::string::iterator it = extension.begin(); it != extension.end(); ++it) {
        *it = static_cast<char>(std::tolower(static_cast<unsigned char>(*it)));
    }

    std::map<std::string, std::string> mimeTypes;
    mimeTypes["html"] = "text/html";
    mimeTypes["css"] = "text/css";
    mimeTypes["js"] = "application/javascript";
    mimeTypes["jpg"] = "image/jpg";
    mimeTypes["jpeg"] = "image/jpeg";
    mimeTypes["png"] = "image/png";
    mimeTypes["gif"] = "image/gif";
    mimeTypes["svg"] = "image/svg+xml";
    mimeTypes["mp4"] = "video/mp4";

    std::map<std::string, std::string>::iterator it = mimeTypes.find(extension);
    if (it != mimeTypes.end()) {
        return "Content-Type: " + it->second;
    } else {
        return "Content-Type: application/octet-stream";
    }
}


std::string HttpResponseBuilder::_makeDateHeader(void)
{
    std::ostringstream oss;
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];

    tstruct = *gmtime(&now);
    strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S %Z", &tstruct);
    oss << "Date: " << buf;
    return (oss.str());
}

HttpResponseBuilder::HttpResponseBuilder(ft::shared_ptr<Client> client) : 
_serverHeader("Server: webserv 1.1"),
_dateHeader(HttpResponseBuilder::_makeDateHeader()),
_KeepAliveHeader("Keep-Alive: timeout=30"),
_client(client)
{

}

HttpResponseBuilder::~HttpResponseBuilder(void)
{

}

void HttpResponseBuilder::_setStatusCode(const HttpStatusCode &statusCode)
{
    this->_statusCode = statusCode;
}
void HttpResponseBuilder::_allocContentLength(ContentLength::e_content_length_header_type contentLengthHeaderType,
                             int contentLength)
{
    if (contentLengthHeaderType == ContentLength::e_content_length_header)
        this->_contentLength = ft::Optional<ContentLength>(ContentLength(contentLength));
    else
        this->_contentLength = ft::Optional<ContentLength>(ContentLength());
}

void HttpResponseBuilder::_buildDefaultResponseHeader(std::vector<char> &buffer)
{
    std::ostringstream oss;
    if (this->_statusCode.has_value() == false)
        throw std::runtime_error("HttpResponseBuilder::_buildResponseHeader: statusCode is not set");
    
    oss << "HTTP/1.1 " << this->_statusCode.value() << " " << 
    HttpStatus::getReasonPhrase(this->_statusCode.value()) << "\r\n";

    oss << this->_serverHeader << "\r\n";

    oss << this->_dateHeader << "\r\n";

//Todo: hyunkyle님한테 이거 맞냐고 물어봐야됨 
    if (this->_client->isClientDie() == true || this->_client->getRequest()->getHeader("Connection") == "close")
        this->_connectionHeader = "Connection: close";
    else
        this->_connectionHeader = "Connection: keep-alive";
    oss << this->_connectionHeader << "\r\n";

    if (this->_client->isClientDie() == false)
        oss << this->_KeepAliveHeader << "\r\n";

    if (this->_contentLength.has_value() == false)
        throw std::runtime_error("HttpResponseBuilder::_buildResponseHeader: contentLength is not set");

    oss << this->_contentLength->getContentLengthHeader() << "\r\n";

    std::string header = oss.str();
    buffer.insert(buffer.end(), header.begin(), header.end());
}

ft::shared_ptr<Client> HttpResponseBuilder::getClient(void)
{
    return (this->_client);
}