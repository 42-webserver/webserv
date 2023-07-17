#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include "../Utils/HttpStatus.hpp"
#include "../Utils/Cookie.hpp"
#include <iostream>

class HttpResponse
{
private:
	std::string 		_version;
	std::vector<char>	_body;
	std::string 		_protocol;
	HttpStatusCode 		_statusCode;
	std::string 		_fileName;
	
	std::multimap<std::string, std::string> _headers;
	std::vector<Cookie> _cookies;

public:
	HttpResponse();
	~HttpResponse();
	void setBody(std::vector<char> & body);
	void setBody(std::string & body);
	void setFileName(std::string & fileName);
	void addHeader(const std::string & key, const std::string & value);
	void setStatusCode(HttpStatusCode code);
	void addCookie(const std::string & key, const std::string & value);
	std::string getVersion();
	std::string getFileName();
	std::vector<char> &getBody();
	std::string getProtocol();
	std::string getReasonPhrase();
	int getStatusCode();
	const std::multimap<std::string, std::string> &getHeaders() const;
	std::string getHeader(const std::string & key);
	const std::vector<Cookie> &getCookies() const;

	friend	std::ostream &operator<<(std::ostream & os, const HttpResponse & response);

};

#endif
