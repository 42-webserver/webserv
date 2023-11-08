#ifndef ERRORPAGEBUILDER_HPP
# define ERRORPAGEBUILDER_HPP

#include <iostream>
#include "HttpResponseBuilder.hpp"
#include <Http/Utils/HttpStatus.hpp>

class ErrorPageBuilder: public HttpResponseBuilder
{
private:
    HttpStatusCode _status;
public:
    ErrorPageBuilder(ft::shared_ptr<Client> client, HttpStatusCode status);
    virtual ~ErrorPageBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);
    static std::string getErrorPage(HttpStatusCode status);
};
#endif