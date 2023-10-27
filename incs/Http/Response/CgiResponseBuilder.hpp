#ifndef CGIRESPONSEBUILDER_HPP
# define CGIRESPONSEBUILDER_HPP

#include <map>
#include <iostream>
#include "HttpResponseBuilder.hpp"
#include <set>

class CgiResponseBuilder: public HttpResponseBuilder
{
private:
    std::map<std::string, std::string> &_headers;
public:
    CgiResponseBuilder(std::map<std::string, std::string> &headers,
        ft::shared_ptr<Client> client);
    virtual ~CgiResponseBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);
private:
    void _handleStatusCode();
};

#endif