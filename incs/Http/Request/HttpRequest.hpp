#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "../Exception/BadRequestException.hpp"
#include "MultipartRequest.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
#include "../../../libs/shared_ptr/shared_ptr.hpp"

typedef enum BodyType{
		NORMAL,
		CHUNKED,
		MULTIPART_FORM_DATA
} BodyType;

class HttpRequest
{
private:
	bool									_error;
	HttpStatusCode							_errorStatusCode;
	std::string		 						_method;
	std::string		 						_uri;
	std::string		 						_version;
	ft::shared_ptr<IoReadAndWriteBuffer>    _body;
	std::string		 						_protocol;
	BodyType								_bodyType;
	bool									_pending;

	std::multimap<std::string, std::string> _headers;
	std::map<std::string, std::string>	  	_queries;
	std::map<std::string, std::string>	  	_cookies;
	std::vector<MultipartRequest>		 	_multipartRequests;

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
	ft::shared_ptr<IoReadAndWriteBuffer> getBody();
	std::vector<MultipartRequest>	&getMultipartRequests();
	std::multimap<std::string, std::string> &getHeaders();
	std::map<std::string, std::string> getQueries();
	std::map<std::string, std::string> getCookies();
	int  getContentLength();
	BodyType getBodyType();
	void setBodyType(BodyType bodyType);
	std::string getHeader(const std::string & key);
	bool isPending();
	void setFinished();
	bool isError();
	void setError(HttpStatusCode errorCode);
	HttpStatusCode getErrorStatusCode();
	friend std::ostream &operator<<(std::ostream & os, const HttpRequest & request);

private:
	void setQuery(std::string & uri);
	void setCookie(std::string & cookie);
	bool esacpeComma(std::string & value);
	void handleMultipleValueHeader(std::string & value, std::string & key);
};

#endif