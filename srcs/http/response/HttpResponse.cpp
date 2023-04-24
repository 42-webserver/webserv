#include "HttpResponse.hpp"


HttpResponse::HttpResponse()
{
    this->_version = "1.1";
    this->_protocol = "HTTP";
    this->setStatusCode(OK);
}

void HttpResponse::setBody(const std::string & body)
{
    this->_body = body;
}

void HttpResponse::addHeader(const std::string & key, const std::string & value)
{
    this->_headers.insert(std::pair<std::string, std::string>(key, value));
}

void HttpResponse::setStatusCode(HttpStatusCode code)
{
    this->_statusCode = code;
}

const std::string &HttpResponse::getVersion()
{
    return (std::string &)this->_version;
}

const std::string &HttpResponse::getBody()
{
    return (std::string &)this->_body;
}

const std::string &HttpResponse::getProtocol()
{
    return (std::string &)this->_protocol;
}

const std::string &HttpResponse::getReasonPhrase()
{
    return HttpStatus::getReasonPhrase(this->_statusCode);
}

int HttpResponse::getStatusCode()
{
    return HttpStatus::getStatusCode(this->_statusCode);
}

const std::multimap<std::string, std::string> &HttpResponse::getHeaders() const
{
    return (std::multimap<std::string, std::string> &)this->_headers;
}