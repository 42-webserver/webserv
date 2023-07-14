#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>

typedef enum BodyType{
		NORMAL,
		CHUNKED,
		MULTIPART_FORM_DATA
} BodyType;

static const int				_MEMORY_BODY_SIZE = 16 * 1024;

class HttpRequest
{
private:
	std::string		 	_method;
	std::string		 	_uri;
	std::string		 	_version;
	std::vector<char>   _body;
	std::string		 	_protocol;
	std::string		 	_bodyDataFilename;
	bool				_isBodyLong;
	BodyType			_bodyType;

	std::multimap<std::string, std::string> _headers;
	std::map<std::string, std::string>	  	_queries;
	std::map<std::string, std::string>	  	_cookies;

public:
	HttpRequest();
	~HttpRequest();
	void addHeader(const std::string & header);
	void setStartLine(std::string line);
	void insertBody(std::vector<char> &buffer);
	std::string getMethod();
	std::string getUri();
	std::string getVersion();
	std::string getProtocol();
	std::string getHost();
	std::vector<char> &getBody();
	std::multimap<std::string, std::string> &getHeaders();
	std::map<std::string, std::string> getQueries();
	std::map<std::string, std::string> getCookies();
	bool isBodyLong() const;
	std::string &getBodyDataFilename();
	void setBodyDataFilename(std::string filename);
	void setBodyLong(bool isBodyLong);
	int  getContentLength();
	BodyType getBodyType();
	void setBodyType(BodyType bodyType);
	std::string getHeader(const std::string & key);

private:
	void setQuery(std::string & uri);
	void setCookie(std::string & cookie);
	bool esacpeComma(std::string & value);
	void handleMultipleValueHeader(std::string & value, std::string & key);
};

#endif