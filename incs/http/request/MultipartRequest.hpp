#ifndef MULTIPART_REQUEST_HPP
#define MULTIPART_REQUEST_HPP

#include <string>
#include <map>
#include <vector>

class MultipartRequest
{
private:
	std::string		 	                    _bodyDataFilename;
    std::vector<char>                       _memoryBody;
    std::multimap<std::string, std::string> _headers;

public:
    MultipartRequest(void);
    ~MultipartRequest(void);
    void addHeader(const std::string & header);
    std::string & getBodyDataFilename();
    void insertBody(std::vector<char> &buffer);
    void setBodyDataFilename(std::string filename);
    std::multimap<std::string, std::string> &getHeaders();
    std::vector<char> &getMemoryBody();

private:
    void handleMultipleValueHeader(std::string & value, std::string & key);
};



#endif