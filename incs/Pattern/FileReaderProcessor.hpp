#ifndef FILEREADERPROCESSOR_HPP
# define FILEREADERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>
#include <Http/Response/GetResponseBuilder.hpp>
class FileReaderProcessor : public ProcessorObject {
    public:
        FileReaderProcessor(void);
        virtual ~FileReaderProcessor(void);
        virtual e_pattern_Process_result process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client,
            ft::shared_ptr<Channel> channel);
        virtual e_pattern_Process_result querryCanSending(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
};
#endif