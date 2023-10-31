#include <Pattern/FileReaderProcessor.hpp>

// To do: implement FileReaderProcessor
FileReaderProcessor::FileReaderProcessor(void) {
}
FileReaderProcessor::~FileReaderProcessor(void) {
}
e_pattern_Process_result FileReaderProcessor::process(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client,
    ft::shared_ptr<Channel> channel) {
    (void)channel;
    FileManager& fileManager = FileManager::getInstance();
    e_FileRequestType type;
    //Todo : try check
    try {
        //basic Handle All RequireMent
        std::string indexingPath = RouterUtils::findPriorityPathWithIndex(virtualServerManager, client->getRequest());
        std::cerr << "indexingPath : " << indexingPath << std::endl;
        _commandBuildHeaderTo(
            ft::shared_ptr<HttpResponseBuilder>(new GetResponseBuilder(client, virtualServerManager, indexingPath)),
            client
        );

        //Todo: 여기서부터 로직한번더 체크해야됨
        type = fileManager.requstFileContent(indexingPath, client->getResponse());
    }
    catch (DirectoryException& e) {
        std::cerr << e.what() << std::endl;
        return SUCCESS;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        //log error
        throw ;
    }
    
    switch (type)
    {
        case FileRequestSuccess: {
            return SUCCESS;
        }
        // FileRequestShouldwait ->block
        default:
            return WAITING;
    }
}

e_pattern_Process_result FileReaderProcessor::querryCanSending(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
    FileManager& fileManager = FileManager::getInstance();
    e_FileRequestType type;
    std::string indexingPath = RouterUtils::findPriorityPathWithIndex(virtualServerManager, client->getRequest());
    type = fileManager.requstFileContent(indexingPath, client->getResponse());
    switch (type)
    {
        case FileRequestSuccess:
            return SUCCESS;
        break;
        default:
            return WAITING;
    }
}
