#ifndef FILEREADERPROCESSOR_HPP
# define FILEREADERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>

class FileReaderProcessor : public ProcessorObject {
    public:
        FileReaderProcessor(void);
        virtual ~FileReaderProcessor(void);
        virtual void process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<HttpRequest> request);
};
#endif