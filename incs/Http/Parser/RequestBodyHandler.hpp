#ifndef REQUEST_BODY_HANDLER_HPP
#define REQUEST_BODY_HANDLER_HPP

#include <vector>
#include "../Request/HttpRequest.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "FileNameGenerator.hpp"
#include "ParsePatternMatcher.hpp"

class RequestBodyHandler
{

protected:
int 						_readBodySize;
ft::shared_ptr<HttpRequest> _request;
 
public:
	//true -> parse finish, false -> parse not finish
	virtual bool handleBody(std::vector<char> &buffer) = 0;
	virtual ~RequestBodyHandler(void) = 0;

protected:
	RequestBodyHandler(int readBodySize, ft::shared_ptr<HttpRequest> req);
};

#endif