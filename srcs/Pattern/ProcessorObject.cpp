#include <Pattern/ProcessorObject.hpp>
#include <Buffer/Node/NormalNode.hpp>
ProcessorObject::ProcessorObject(void) {
}
ProcessorObject::~ProcessorObject(void) {
}
void ProcessorObject::_commandBuildHeaderTo(ft::shared_ptr<HttpResponseBuilder> builder,
ft::shared_ptr<Client> client){
    // client->getResponse()->allocateBuilder(builder);
        builder->buildResponseHeader(client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey()));
        ssize_t contentLength = builder->getContentLength();

        if (contentLength > 0) {
            if (contentLength <= NormalNode::_sizeNormal) {
                client->getResponse()->setResponseSize(NormalSize, HttpResponse::AccessKey());
            }
            else {
                client->getResponse()->setResponseSize(BigSize, HttpResponse::AccessKey());
            }
        }
        else {
            client->getResponse()->setResponseSize(NormalSize, HttpResponse::AccessKey());
        }
}

