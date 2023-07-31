#ifndef MULTIPART_REQUEST_BODY_HANDELR_HPP
#define MULTIPART_REQUEST_BODY_HANDELR_HPP

#include "RequestBodyHandler.hpp"
#include <algorithm>
#include <map>
#include <string>
#include "ParsePatternMatcher.hpp"
#include "../Utils/FileUploader.hpp"
#include "../Utils/RouterUtils.hpp"
#include "../../Server/VirtualServerManager.hpp"

typedef enum MultipartState{
	M_HEADER,
	M_BODY,
	M_END
} MultipartParseState;

class MultipartRequestBodyHandler : public RequestBodyHandler {

private:
	std::vector<char>		   				_buffer;
	std::string				 				_boundaryStart;
	std::string				 				_boundaryEnd;
	MultipartParseState		 				_state;
	int										_index;

public:
	bool handleBody(std::vector<char> &buffer);
	MultipartRequestBodyHandler(std::string boundary, ft::shared_ptr<HttpRequest> req);
	~MultipartRequestBodyHandler(void);

private:
	void parseParts(std::vector<char> &partBody);
	void allocateMultiparts(int count);
	int  countPartSize(std::vector<char> &partBody);
	void parsePartOfBody(std::vector<char> &partBody);
	void handleMultipartHeader(std::vector<char> &partBody);
	void addBody(std::vector<char> &body);
	void addHeader(const std::string &line);
	void handleMultipleValueHeader(std::string &value, std::string &key);
};

#endif
