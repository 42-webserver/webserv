#include "../../../incs/Http/Response/HttpResponse.hpp"


HttpResponse::HttpResponse()
{
	this->_version = "1.1";
	this->_protocol = "HTTP";
	this->setStatusCode(OK);
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::setBody(std::vector<char> & body)
{
	this->_body.insert(this->_body.end(), body.begin(), body.end());
}

void HttpResponse::setBody(std::string & body)
{
	this->_body.insert(this->_body.end(), body.begin(), body.end());
}

void HttpResponse::setFileName(std::string & fileName)
{
	this->_fileName = fileName;
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

std::string HttpResponse::getFileName()
{
	return this->_fileName;
}

std::vector<char> &HttpResponse::getBody()
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

std::string HttpResponse::getHeader(const std::string & key)
{
	std::multimap<std::string, std::string>::iterator it = this->_headers.find(key);
	if (it != this->_headers.end())
		return it->second;
	return "";
}

const std::multimap<std::string, std::string> &HttpResponse::getHeaders() const
{
	return (std::multimap<std::string, std::string> &)this->_headers;
}


std::ostream &operator<<(std::ostream & os,const HttpResponse & response){
	os << "Version: " << response._version << std::endl;
	os << "Protocol: " << response._protocol << std::endl;
	os << "StatusCode: " << response._statusCode << std::endl;

	os << "Headers: " << std::endl;
	for (std::multimap<std::string, std::string>::const_iterator it = response._headers.begin(); it != response._headers.end(); it++)
		os << it->first << ": " << it->second << std::endl;
	os << "Cookies: " << std::endl;
	for (std::vector<Cookie>::const_iterator it = response._cookies.begin(); it != response._cookies.end(); it++)
		os << *it << std::endl;
	if (response._body.size() > 0){
		os << "Body: " << std::endl;
		for (std::vector<char>::const_iterator it = response._body.begin(); it != response._body.end(); it++)
			os << *it;
		os << std::endl;
	}else{
		os << "BodyFilename: " << response._fileName << std::endl;
	}
	return os;
}