#ifndef GETRESPONSEBUILDER_HPP
# define GETRESPONSEBUILDER_HPP

#include <iostream>
#include "HttpResponseBuilder.hpp"


class GetResponseBuilder: public HttpResponseBuilder
{
private:
    std::string _ContentTypeHeader;
    ft::shared_ptr<VirtualServerManager> _vsm;
    std::string _indexingPath;
public:
    GetResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm,
        std::string indexingPath);
    virtual ~GetResponseBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);
};

#endif