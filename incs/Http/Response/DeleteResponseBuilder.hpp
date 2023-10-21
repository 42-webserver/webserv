#ifndef DELETERESPONSEBUILDER_HPP
# define DELETERESPONSEBUILDER_HPP

#include <iostream>
#include "HttpResponseBuilder.hpp"

class DeleteResponseBuilder: public HttpResponseBuilder
{
private:
    ft::shared_ptr<VirtualServerManager> _vsm;

public:
    DeleteResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm);
    virtual ~DeleteResponseBuilder(void);
    virtual void buildResponseHeader(std::vector<char> &buffer);

};

#endif