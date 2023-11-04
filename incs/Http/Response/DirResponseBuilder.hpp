#ifndef DIRRESPONSEBUILDER_HPP
# define DIRRESPONSEBUILDER_HPP

#include <iostream>
#include "HttpResponseBuilder.hpp"

class DirResponseBuilder: public HttpResponseBuilder
{
private:
    std::string _ContentTypeHeader;
    ft::shared_ptr<VirtualServerManager> _vsm;
public:
    DirResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm);
    virtual ~DirResponseBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);
};

#endif