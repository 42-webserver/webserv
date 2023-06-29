#include "../../../incs/Http/Response/HttpResponse.hpp"


HttpResponse::HttpResponse()
{
	this->_version = "1.1";
	this->_protocol = "HTTP";
	this->setStatusCode(OK);
	this->_body = NULL;
}

HttpResponse::~HttpResponse()
{
	if(this->_body != NULL)
		delete _body;
}

void HttpResponse::setBody(std::string * body)
{
	this->_body = body;
}

void HttpResponse::addCookie(const std::string & key, const std::string & value)
{
	Cookie cookie(key, value);
	this->_cookies.push_back(cookie);
}

void HttpResponse::addHeader(const std::string & key, const std::string & value)
{
	this->_headers.insert(std::pair<std::string, std::string>(key, value));
}

void HttpResponse::setStatusCode(HttpStatusCode code)
{
	this->_statusCode = code;
}

std::string HttpResponse::getVersion()
{
	return (std::string &)this->_version;
}

std::string *HttpResponse::getBody()
{
	return this->_body;
}

const std::vector<Cookie> &HttpResponse::getCookies() const
{
	return (std::vector<Cookie> &)this->_cookies;
}

std::string HttpResponse::getProtocol()
{
	return (std::string &)this->_protocol;
}

std::string HttpResponse::getReasonPhrase()
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
