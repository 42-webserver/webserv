#include <Pattern/CgiReaderProcessor.hpp>

// To do: implement CgiReaderProcessor
CgiReaderProcessor::CgiReaderProcessor(void) {
}
CgiReaderProcessor::~CgiReaderProcessor(void) {
}
e_pattern_Process_result CgiReaderProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    (void)virtualServerManager;
    (void)client;
    return SUCCESS;
}