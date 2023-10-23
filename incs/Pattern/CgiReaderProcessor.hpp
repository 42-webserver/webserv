#ifndef CGIREADERPROCESSOR_HPP
# define CGIREADERPROCESSOR_HPP

#include <Pattern/ProcessorObject.hpp>

class CgiReaderProcessor : public ProcessorObject {
    public:
        CgiReaderProcessor(void);
        virtual ~CgiReaderProcessor(void);
        virtual e_pattern_Process_result process(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
        virtual e_pattern_Process_result querryCanSending(ft::shared_ptr
            <VirtualServerManager> virtualServerManager,
            ft::shared_ptr<Client> client);
};
#endif