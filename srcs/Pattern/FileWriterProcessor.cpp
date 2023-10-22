#include <Pattern/FileWriterProcessor.hpp>

// To do: implement FileWriterProcessor
FileWriterProcessor::FileWriterProcessor(void) {
}
FileWriterProcessor::~FileWriterProcessor(void) {
}
e_pattern_Process_result FileWriterProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    
    FileManager& fileManager = FileManager::getInstance();
    e_FileRequestType type;
    try {
        client->getResponse()->allocateBuilder(new PutAndPostResponseBuilder(client, virtualServerManager));
        ft::shared_ptr<HttpResponseBuilder> builder = client->getResponse()->getBuilder();
        builder->buildResponseHeader(client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey()));

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
        //Todo: set response
        //send response
            return SUCCESS;
        break;
        case FileRequestFail: 
            //log error
            throw std::runtime_error("fileWriterProcessor: file request fail unknown");
        default:
            return WAITING;
           }
}