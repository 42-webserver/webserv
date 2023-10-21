#ifndef REDIRECTIONRESPONSEBUILDER_HPP
# define REDIRECTIONRESPONSEBUILDER_HPP

#include <iostream>
#include "HttpResponseBuilder.hpp"

class RedirectionResponseBuilder: public HttpResponseBuilder
{
private:
    std::string _locationHeader;
    ft::shared_ptr<VirtualServerManager> _vsm;
public:
    RedirectionResponseBuilder(ft::shared_ptr<Client> client,
    ft::shared_ptr<VirtualServerManager> vsm);
    virtual ~RedirectionResponseBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);

};

#endif