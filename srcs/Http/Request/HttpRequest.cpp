#include "../../../incs/Http/Request/HttpRequest.hpp"


HttpRequest::HttpRequest()
:_isBodyLong(false), _bodyType(NORMAL)
{
	this->_body.reserve(_MEMORY_BODY_SIZE);
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::insertBody(std::vector<char> &buffer)
{
	this->_body.insert(this->_body.end(), buffer.begin(), buffer.end());
}

bool HttpRequest::isBodyLong() const
{
	return this->_isBodyLong;
}

std::string & HttpRequest::getBodyDataFilename(){
	return this->_bodyDataFilename;
}

void HttpRequest::setBodyDataFilename(std::string filename){
	this->_bodyDataFilename = filename;
}

void HttpRequest::setBodyLong(bool isBodyLong){
	this->_isBodyLong = isBodyLong;
}

void HttpRequest::addHeader(const std::string & header)
{
	std::string::size_type pos;
	std::string key;
	std::string value;

	pos = header.find(": ");
	key = header.substr(0, pos);
	value = header.substr(pos + 2);
	handleMultipleValueHeader(value, key);
	if (key == "Cookie")
		setCookie(value);
}

void HttpRequest::handleMultipleValueHeader(std::string & value, std::string & key)
{
	std::string::size_type pos = value.find(",");

	while (pos != std::string::npos)
	{
		if (key == "User-Agent")
			break;
		this->_headers.insert(std::pair<std::string, std::string>(key, value.substr(0, pos)));
		value.erase(0, pos + 1);
		pos = value.find(",");
	}
	this->_headers.insert(std::pair<std::string, std::string>(key, value));
}

void HttpRequest::setCookie(std::string & cookie)
{
	std::string::size_type pos;
	std::string key, value, tmp;

	while (cookie.find("; ") != std::string::npos)
	{
		pos = cookie.find("; ");
		tmp = cookie.substr(0, pos);
		cookie.erase(0, pos + 2);
		key = tmp.substr(0, tmp.find("="));
		value = tmp.substr(tmp.find("=") + 1);
		this->_cookies.insert(std::pair<std::string, std::string>(key, value));
	}
	key = cookie.substr(0, cookie.find("="));
	value = cookie.substr(cookie.find("=") + 1);
	this->_cookies.insert(std::pair<std::string, std::string>(key, value));
}

void HttpRequest::setStartLine(std::string line)
{
	std::string::size_type pos;

	pos = line.find(" ");
	this->_method = line.substr(0, pos);
	line.erase(0, pos + 1);
	pos = line.find(" ");
	this->_uri = line.substr(0, pos);
	if (this->_uri.find("?") != std::string::npos)
		setQuery(this->_uri);
	line.erase(0, pos + 1);
	this->_protocol = line.substr(0, line.find("/"));
	line.erase(0, line.find("/") + 1);
	this->_version = line;
}

void HttpRequest::setQuery(std::string & uri)
{
	std::string::size_type pos;
	std::string key, value, query, tmp;

	query = uri.substr(uri.find("?") + 1);
	while (query.find("&") != std::string::npos){
		pos = query.find("&");
		tmp = query.substr(0, pos);
		query.erase(0, pos + 1);
		value = tmp.substr(tmp.find("=") + 1);
		key = tmp.substr(0, tmp.find("="));
		this->_queries.insert(std::pair<std::string, std::string>(key, value));
	}
	key = query.substr(0, query.find("="));
	value = query.substr(query.find("=") + 1);
	this->_queries.insert(std::pair<std::string, std::string>(key, value));
}

std::string HttpRequest::getMethod()
{
	return this->_method;
}

std::string HttpRequest::getUri()
{
	return this->_uri;
}

std::string HttpRequest::getVersion()
{
	return this->_version;
}

std::vector<char> &HttpRequest::getBody()
{
	return this->_body;
}

std::string HttpRequest::getProtocol()
{
	return this->_protocol;
}

std::multimap<std::string, std::string> & HttpRequest::getHeaders()
{
	return this->_headers;
}

std::map<std::string, std::string> HttpRequest::getQueries()
{
	return this->_queries;
}

std::map<std::string, std::string> HttpRequest::getCookies()
{
	return this->_cookies;
}

int HttpRequest::getContentLength()
{
	std::multimap<std::string, std::string>::iterator it;

	it = this->_headers.find("Content-Length");
	if (it == this->_headers.end())
		return -1;
	return std::stoi(it->second);
}

std::string HttpRequest::getHost()
{
	std::multimap<std::string, std::string>::iterator it;

	it = this->_headers.find("Host");
	if (it == this->_headers.end())
		return "";
	return it->second;
}

BodyType HttpRequest::getBodyType()
{
	return this->_bodyType;
}

void HttpRequest::setBodyType(BodyType bodyType)
{
	this->_bodyType = bodyType;
}