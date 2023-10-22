#include <Http/Response/RedirectionResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>

RedirectionResponseBuilder::RedirectionResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm) :
HttpResponseBuilder(client), _vsm(vsm) {}

RedirectionResponseBuilder::~RedirectionResponseBuilder(void) {}

void RedirectionResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    // RouterUtils::findRedirectUri
    (void)buffer;
    //Todo: findRedirectUri 완성되면 코드 
}