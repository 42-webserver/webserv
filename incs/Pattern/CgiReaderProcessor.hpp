#ifndef CGIREADERPROCESSOR_HPP
# define CGIREADERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>

class CgiReaderProcessor : public ProcessorObject {
    public:
        CgiReaderProcessor(void);
        virtual ~CgiReaderProcessor(void);
        virtual void process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<HttpRequest> request);
};
#endif