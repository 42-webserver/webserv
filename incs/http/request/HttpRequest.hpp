#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>

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

	std::multimap<std::string, std::string> _headers;
	std::map<std::string, std::string>	  _queries;
	std::map<std::string, std::string>	  _cookies;

public:
	HttpRequest();
	~HttpRequest();
	void addHeader(const std::string & header);
	void setStartLine(std::string line);
	void setBody(std::vector<char> &buffer);
	std::string getMethod();
	std::string getUri();
	std::string getVersion();
	std::string getProtocol();
	std::vector<char> &getBody();
	std::multimap<std::string, std::string> &getHeaders();
	std::map<std::string, std::string> getQueries();
	std::map<std::string, std::string> getCookies();
	bool isBodyLong() const;
	std::string &getBodyDataFilename();
	void setBodyDataFilename(std::string filename);
	void setBodyLong(bool isBodyLong);

private:
	void setQuery(std::string & uri);
	void setCookie(std::string & cookie);
	bool esacpeComma(std::string & value);
	void handleMultipleValueHeader(std::string & value, std::string & key);
};

#endif