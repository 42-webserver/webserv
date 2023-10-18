#ifndef FILEDELETERPROCESSOR_HPP
# define FILEDELETERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>

class FileDeleterProcessor : public ProcessorObject {
    public:
        FileDeleterProcessor(void);
        virtual ~FileDeleterProcessor(void);
        virtual void process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
};
#endif