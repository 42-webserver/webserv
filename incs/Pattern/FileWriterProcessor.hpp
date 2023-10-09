#ifndef FileWriterProcessor_HPP
# define FileWriterProcessor_HPP

#include <Pattern/ProcessorObject.hpp>

class FileWriterProcessor : public ProcessorObject {
    public:
        FileWriterProcessor(void);
        virtual ~FileWriterProcessor(void);
        virtual void process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<HttpRequest> request);
};
#endif