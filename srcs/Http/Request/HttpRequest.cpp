#include "../../../incs/Http/Request/HttpRequest.hpp"
#include <Buffer/Buffer/IoOnlyReadBuffer.hpp>

//debug
void HttpRequest::_printBody()
{
	std::cerr << "Method: " << this->_method << std::endl;
	std::cerr << "Uri: " << this->_uri << std::endl;
	std::cerr << "Version: " << this->_version << std::endl;
	std::cerr << "Protocol: " << this->_protocol << std::endl;
	std::cerr << "BodyType: " << this->_bodyType << std::endl;
	std::cerr << "Body: " << std::endl;
	this->_body->printBuffer();
	
	
	std::cerr << std::endl;
	std::cerr << "Headers: " << std::endl;
	for (std::multimap<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
		std::cerr << it->first << ": " << it->second << std::endl;
	std::cerr << "Queries: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_queries.begin(); it != this->_queries.end(); it++)
		std::cerr << it->first << ": " << it->second << std::endl;
	std::cerr << "Cookies: " << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_cookies.begin(); it != this->_cookies.end(); it++)
		std::cerr << it->first << ": " << it->second << std::endl;
}



// FileUpload 

HttpRequest::AccessKey::AccessKey()
{
}

HttpRequest::AccessKey::~AccessKey()
{
}

void HttpRequest::setFileUploadSync(e_file_upload_sync fileSync, AccessKey key)
{
	(void)key;
	this->_fileUploadSync = fileSync;
}

e_file_upload_sync HttpRequest::getFileUploadSync(AccessKey key)
{
	(void)key;
	return this->_fileUploadSync;
}
// End FileUpload



HttpRequest::HttpRequest():
	_error(false), _bodyType(NORMAL)
{
	_body = ft::make_shared<IoReadAndWriteBuffer>();
}

HttpRequest::~HttpRequest()
{
}

ssize_t HttpRequest::insertBody(std::vector<char> &buffer)
{
	return (this->_body->append(buffer.begin(), buffer.end()));
}

ssize_t HttpRequest::insertBody(void )
{
	IoOnlyReadBuffer &readBuffer = IoOnlyReadBuffer::getInstance();

	return (this->_body->append(readBuffer.begin(), readBuffer.end()));
}

//Todo: 여기서 공백하나라고 가정하는거 위험
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
	std::string method;
	pos = line.find(" ");
	method = line.substr(0, pos);
	std::transform(method.begin(), method.end(),method.begin(), ::toupper);
	this->_method = method;
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

void HttpRequest::setUri(std::string uri)
{
	this->_uri.clear();
	this->_uri = uri;
}

std::string HttpRequest::getVersion()
{
	return this->_version;
}

ft::shared_ptr<IoReadAndWriteBuffer> HttpRequest::getBody()
{
	return this->_body;
}

size_t HttpRequest::getBodySize() const
{
	return this->_body->size();
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

ssize_t HttpRequest::getContentLength()
{
	std::multimap<std::string, std::string>::iterator it;

	//fix : daegulee
	it = this->_headers.find("Content-Length");
	if (it == this->_headers.end())
		return noContentLength;
	return std::stoul(it->second);
}

std::vector<MultipartRequest> & HttpRequest::getMultipartRequests()
{
	return this->_multipartRequests;
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

std::string HttpRequest::getHeader(const std::string & key)
{
	std::multimap<std::string, std::string>::iterator it = this->_headers.find(key);
	if (it != this->_headers.end())
		return it->second;
	return "";
}





bool HttpRequest::isError(){
	return this->_error;
}

void HttpRequest::setError(HttpStatusCode errorCode){
	this->_error = true;
	this->_errorStatusCode = errorCode;
}

HttpStatusCode HttpRequest::getErrorStatusCode(){
	return this->_errorStatusCode;
}

std::ostream &operator<<(std::ostream & os, const HttpRequest & request)
{
		os << "Method: " << request._method << std::endl;
		os << "Uri: " << request._uri << std::endl;
		os << "Version: " << request._version << std::endl;
		os << "Protocol: " << request._protocol << std::endl;
		
		if (request._bodyType == NORMAL)
			os << "BodyType: NORMAL" << std::endl;
		else if (request._bodyType == CHUNKED)
			os << "BodyType: CHUNKED" << std::endl;
		else if (request._bodyType == MULTIPART_FORM_DATA)
			os << "BodyType: MULTIPART_FORM_DATA" << std::endl;
		else
			os << "BodyType: UNKNOWN" << std::endl;
		// os << "Body: " << std::endl;
		// for (std::vector<char>::const_iterator it = request._body.begin(); it != request._body.end(); it++)
		// 	os << *it;
		// os << std::endl;
		os << "Headers: " << std::endl;
		for (std::multimap<std::string, std::string>::const_iterator it = request._headers.begin(); it != request._headers.end(); it++)
			os << it->first << ": " << it->second << std::endl;
		os << "Queries: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = request._queries.begin(); it != request._queries.end(); it++)
			os << it->first << ": " << it->second << std::endl;
		os << "Cookies: " << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = request._cookies.begin(); it != request._cookies.end(); it++)
			os << it->first << ": " << it->second << std::endl;
		return os;
}

void HttpRequest::setBodyType(BodyType bodyType)
{
	this->_bodyType = bodyType;
}