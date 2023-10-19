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
    std::string path = RouterUtils::findPath(virtualServerManager, client->getRequest());
    e_FileRequestType requestType = fileManager.requestFileUpload(path, client->getRequest());

    switch (requestType)
    {
        case FileRequestSuccess:
        //Todo: set response
        //send response
            return SUCCESS;
        break;
        case FileRequestFail:
            throw std::runtime_error("fileWriterProcessor: file request fail unknown");
        default:
            return WAITING;
           }
}