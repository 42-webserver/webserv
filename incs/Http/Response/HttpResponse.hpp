#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include "../Utils/HttpStatus.hpp"
#include "../Utils/Cookie.hpp"
class HttpResponse
{
private:
	std::string _version;
	std::string *_body;
	std::string _protocol;
	HttpStatusCode _statusCode;
	
	std::multimap<std::string, std::string> _headers;
	std::vector<Cookie> _cookies;

public:
	HttpResponse();
	~HttpResponse();
	void setBody(std::string * body);
	void addHeader(const std::string & key, const std::string & value);
	void setStatusCode(HttpStatusCode code);
	void addCookie(const std::string & key, const std::string & value);
	std::string getVersion();
	std::string *getBody();
	std::string getProtocol();
	std::string getReasonPhrase();
	int getStatusCode();
	const std::multimap<std::string, std::string> &getHeaders() const;
	const std::vector<Cookie> &getCookies() const;
};

#endif
