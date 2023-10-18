#ifndef PROCESSOROBJECT_HPP
# define PROCESSOROBJECT_HPP

#include "../../libs/shared_ptr/shared_ptr.hpp"
#include <Http/Request/HttpRequest.hpp>
#include <Server/VirtualServerManager.hpp>
#include <Client/Client.hpp>
class ProcessorObject {
    public:
        virtual void process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client) = 0;
        virtual ~ProcessorObject(void);
        ProcessorObject(void);
};

#endif 