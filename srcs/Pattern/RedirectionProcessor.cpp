#include <Pattern/RedirectionProcessor.hpp>

// To do: implement RedirectionProcessor
RedirectionProcessor::RedirectionProcessor(void) {
}
RedirectionProcessor::~RedirectionProcessor(void) {
}

e_pattern_Process_result RedirectionProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    
    try {
        client->getResponse()->allocateBuilder(new RedirectionResponseBuilder(client, virtualServerManager));
        ft::shared_ptr<HttpResponseBuilder> builder = client->getResponse()->getBuilder();
        builder->buildResponseHeader(client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey()));
    }
    catch (std::exception& e) {
        //log error
        throw ;
    }
    return SUCCESS;
}
