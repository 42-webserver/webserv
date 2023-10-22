#ifndef FILEDELETERPROCESSOR_HPP
# define FILEDELETERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>
#include <Http/Response/DeleteResponseBuilder.hpp>
class FileDeleterProcessor : public ProcessorObject {
    public:
        FileDeleterProcessor(void);
        virtual ~FileDeleterProcessor(void);
        virtual e_pattern_Process_result process(ft::shared_ptr
            <VirtualServerManager> vsm,
            ft::shared_ptr<Client> client);
};
#endif