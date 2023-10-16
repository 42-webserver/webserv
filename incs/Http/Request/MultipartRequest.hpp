#ifndef MULTIPART_REQUEST_HPP
#define MULTIPART_REQUEST_HPP

#include <string>
#include <map>
#include <vector>
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
#include "../../../libs/shared_ptr/shared_ptr.hpp"

class MultipartRequest
{
private:
	ft::shared_ptr<IoReadAndWriteBuffer>	 _body;
	std::multimap<std::string, std::string>	 _headers;

public:
	MultipartRequest(void);
	~MultipartRequest(void);
	void addHeader(const std::string & header);
	void insertBody(std::vector<char> &buffer);
	ft::shared_ptr<IoReadAndWriteBuffer> getBody(void);
	bool needUpload(void);
	std::string &getFilename(void);
private:
	void handleMultipleValueHeader(std::string & line);
};



#endif