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
        //basic Handle All RequireMent
        _commandBuildHeaderTo(
            ft::shared_ptr<HttpResponseBuilder>(new GetResponseBuilder(client, virtualServerManager)),
            client
        );

        //Todo: 여기서부터 로직한번더 체크해야됨
        std::string path = RouterUtils::findPriorityPathWithIndex(virtualServerManager, client->getRequest());    
        type = fileManager.requstFileContent(path, client->getResponse());
    }
    catch (DirectoryException& e) {
        return SUCCESS;
    }
    catch (std::exception& e) {
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
    (void)virtualServerManager;
    (void)client;
    return SUCCESS;
}
