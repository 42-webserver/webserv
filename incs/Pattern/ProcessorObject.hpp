#ifndef PROCESSOROBJECT_HPP
# define PROCESSOROBJECT_HPP

#include "../../libs/shared_ptr/shared_ptr.hpp"
#include <Http/Request/HttpRequest.hpp>
#include <Server/VirtualServerManager.hpp>
#include <Client/Client.hpp>
#include <FileManager/FileManager/FileManager.hpp>
#include <Http/Utils/RouterUtils.hpp>

class ProcessorObject {
    public:
        virtual e_pattern_Process_result process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client,
            ft::shared_ptr<Channel> channel) = 0;
        virtual e_pattern_Process_result querryCanSending(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client) = 0;
        virtual ~ProcessorObject(void);
        ProcessorObject(void);
    protected:
        void _commandBuildHeaderTo(ft::shared_ptr<HttpResponseBuilder> builder
        , ft::shared_ptr<Client> client);
};

#endif 