#ifndef MULTIPART_REQUEST_BODY_HANDELR_HPP
#define MULTIPART_REQUEST_BODY_HANDELR_HPP

#include "RequestBodyHandler.hpp"
#include <algorithm>
#include <map>
#include <string>
#include "ParsePatternMatcher.hpp"
#include "../Utils/FileUploader.hpp"

typedef enum MultipartState{
	M_HEADER,
	M_BODY
} MultipartParseState;

class MultipartRequestBodyHandler : public RequestBodyHandler {
private:
	typedef struct MultipartRequest{
		std::multimap<std::string, std::string> _headers;
		std::string _filename;
	} MultipartRequset;

private:
	std::vector<char>		   _buffer;
	std::string				 _boundaryStart;
	std::string				 _boundaryEnd;
	MultipartParseState		 _state;
	MultipartRequset		 _multipartRequest;

public:
	bool handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
	MultipartRequestBodyHandler(std::string boundary);
	~MultipartRequestBodyHandler(void);

private:
	void handleMultipartHeader(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
	bool parsePartOfBody(std::vector<char> &reqBuffer);
	void writeParts(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req);
	void addHeader(const std::string &line);
	void handleMultipleValueHeader(std::string &value, std::string &key);
	void generatePath();
};

#endif
