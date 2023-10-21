#ifndef PUTANDPOSTRESPONSEBUILDER_HPP
# define PUTANDPOSTRESPONSEBUILDER_HPP

#include <iostream>
#include "HttpResponseBuilder.hpp"

class PutAndPostResponseBuilder: public HttpResponseBuilder
{
private:
    std::string _locationHeader;
    ft::shared_ptr<VirtualServerManager> _vsm;
public:
    PutAndPostResponseBuilder(ft::shared_ptr<Client> client,
    ft::shared_ptr<VirtualServerManager> vsm);
    virtual ~PutAndPostResponseBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);

};

#endif