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

std::ostream &HttpResponse::operator<<(std::ostream & os)
{
	os << this->_protocol << "/" << this->_version << " " << this->getStatusCode() << " " << this->getReasonPhrase() << std::endl;
	for (std::multimap<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		os << it->first << ": " << it->second << std::endl;
	for (std::vector<Cookie>::iterator it = this->_cookies.begin(); it != this->_cookies.end(); it++)
		os << "Set-Cookie: " << it->getKey() << "=" << it->getValue() << std::endl;
	os << std::endl;
	if (this->_body.size() > 0){
		os << "Body: " << std::endl;
		for (std::vector<char>::iterator it = this->_body.begin(); it != this->_body.end(); it++)
			os << *it;
	}else{
		os << "BodyDataFilename: " << this->_fileName << std::endl;
	}
	return os;
}