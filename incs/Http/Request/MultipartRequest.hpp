#ifndef MULTIPART_REQUEST_HPP
#define MULTIPART_REQUEST_HPP

#include <string>
#include <map>
#include <vector>
class MultipartRequest
{
private:
	std::vector<char>					    _body;
	std::multimap<std::string, std::string> _headers;

public:
	MultipartRequest(void);
	~MultipartRequest(void);
	void addHeader(const std::string & header);
	void insertBody(std::vector<char> &buffer);
	std::multimap<std::string, std::string> &getHeaders();
	std::vector<char> &getBody();

private:
	void handleMultipleValueHeader(std::string & value, std::string & key);
};



#endif