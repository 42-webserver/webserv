#ifndef MULTIPART_REQUEST_HPP
#define MULTIPART_REQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>

class MultipartRequest
{
private:
	IoReadAndWriteBuffer					 _body;
	std::multimap<std::string, std::string>	 _headers;

public:
	MultipartRequest(void);
	~MultipartRequest(void);
	void addHeader(const std::string & header);
	void insertBody(std::vector<char> &buffer);
	std::multimap<std::string, std::string> &getHeaders();
	IoReadAndWriteBuffer &getBody();

private:
	void handleMultipleValueHeader(std::string & value, std::string & key);
};



#endif