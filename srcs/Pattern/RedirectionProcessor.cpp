#include <Pattern/RedirectionProcessor.hpp>

// To do: implement RedirectionProcessor
RedirectionProcessor::RedirectionProcessor(void) {
}
RedirectionProcessor::~RedirectionProcessor(void) {
}

e_pattern_Process_result RedirectionProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    (void)virtualServerManager;
    (void)client;
    return SUCCESS;
}
