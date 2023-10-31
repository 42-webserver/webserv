#include <Pattern/ProcessorObject.hpp>

ProcessorObject::ProcessorObject(void) {
}
ProcessorObject::~ProcessorObject(void) {
}
void ProcessorObject::_commandBuildHeaderTo(ft::shared_ptr<HttpResponseBuilder> builder,
ft::shared_ptr<Client> client){
    // client->getResponse()->allocateBuilder(builder);
        builder->buildResponseHeader(client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey()));
}

