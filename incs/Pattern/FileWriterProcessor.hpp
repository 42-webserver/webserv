#ifndef FileWriterProcessor_HPP
# define FileWriterProcessor_HPP

#include <Pattern/ProcessorObject.hpp>
#include <Http/Response/PutAndPostResponseBuilder.hpp>
class FileWriterProcessor : public ProcessorObject {
    public:
        FileWriterProcessor(void);
        virtual ~FileWriterProcessor(void);
        virtual e_pattern_Process_result process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
        virtual e_pattern_Process_result querryCanSending(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
};
#endif