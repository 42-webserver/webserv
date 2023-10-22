#include <Pattern/FileReaderProcessor.hpp>

// To do: implement FileReaderProcessor
FileReaderProcessor::FileReaderProcessor(void) {
}
FileReaderProcessor::~FileReaderProcessor(void) {
}
e_pattern_Process_result FileReaderProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    FileManager& fileManager = FileManager::getInstance();
    e_FileRequestType type;
    //Todo : try check
    try {
        client->getResponse()->allocateBuilder(new GetResponseBuilder(client, virtualServerManager));
        ft::shared_ptr<HttpResponseBuilder> builder = client->getResponse()->getBuilder();
        builder->buildResponseHeader(client->getResponse()->getNormalCaseBuffer(HttpResponse::AccessKey()));

        //Todo: 여기서부터 로직한번더 체크해야됨
        std::string path = RouterUtils::findPriorityPathWithIndex(virtualServerManager, client->getRequest());    
        type = fileManager.requstFileContent(path, client->getResponse());
    }
    catch (std::exception& e) {
        //log error
        throw ;
    }
    
    switch (type)
    {
        case FileRequestFail: {
            throw std::runtime_error("FileReaderProcessor::process : FileRequestFail");
            return FAILURE;
        };
        case FileRequestSuccess: {
            return SUCCESS;
        }
        // FileRequestShouldwait ->block
        default:
            return WAITING;
    }
}
