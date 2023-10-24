#include <Pattern/FileWriterProcessor.hpp>

// To do: implement FileWriterProcessor
FileWriterProcessor::FileWriterProcessor(void) {
}
FileWriterProcessor::~FileWriterProcessor(void) {
}
e_pattern_Process_result FileWriterProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client,
    ft::shared_ptr<Channel> channel) {
    (void)channel;

    FileManager& fileManager = FileManager::getInstance();
    e_FileRequestType type;
    try {
        //basic Handle All RequireMent
        _commandBuildHeaderTo(
            ft::shared_ptr<HttpResponseBuilder>(new PutAndPostResponseBuilder(client, virtualServerManager)),
            client
        );

        std::string path = RouterUtils::findPath(virtualServerManager, client->getRequest());
        type = fileManager.requestFileUpload(path, client->getRequest());
    }
    catch (std::exception& e) {
        //log error
        throw ;
    }

    switch (type)
    {
        case FileRequestSuccess:
            return SUCCESS;
        break;
        default:
            return WAITING;
    }
}

e_pattern_Process_result FileWriterProcessor::querryCanSending(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    FileManager& fileManager = FileManager::getInstance();
    e_FileRequestType type;
    std::string path = RouterUtils::findPath(virtualServerManager, client->getRequest());
    type = fileManager.requestFileUpload(path, client->getRequest());
    switch (type)
    {
        case FileRequestSuccess:
            return SUCCESS;
        break;
        default:
            return WAITING;
    }
}