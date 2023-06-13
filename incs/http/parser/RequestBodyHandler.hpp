#ifndef REQUEST_BODY_HANDLER_HPP
#define REQUEST_BODY_HANDLER_HPP

#include <vector>
#include "../request/HttpRequest.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include "ParsePatternMatcher.hpp"
#include "FileNameGenerator.hpp"
#include "ParsePatternMatcher.hpp"

class RequestBodyHandler
{

protected:
int _readBodySize;
 
public:
    //true -> parse finish, false -> parse not finish
    virtual bool handleBody(std::vector<char> &reqBuffer, ft::shared_ptr<HttpRequest> req) = 0;
    virtual ~RequestBodyHandler(void) = 0;

protected:
    RequestBodyHandler(int readBodySize);
};

#endif