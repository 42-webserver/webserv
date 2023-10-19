#include <Pattern/FileDeleterProcessor.hpp>
// To do: implement FileDeleterProcessor
FileDeleterProcessor::FileDeleterProcessor(void) {
}
FileDeleterProcessor::~FileDeleterProcessor(void) {
}
e_pattern_Process_result FileDeleterProcessor::process(ft::shared_ptr
    <VirtualServerManager> vsm,
    ft::shared_ptr<Client> client) {
   FileManager& fileManager = FileManager::getInstance();
   std::string path = RouterUtils::findPath(vsm, client->getRequest());
   e_FileRequestType type = fileManager.requestFileDelete(path, client->getResponse());

   switch (type)
   {
        case FileRequestFail: {
    //To do: send error page, 상태 코드 들어있음, 그냥 그거 보고 리스폰스 만들어서 송신
          return FAILURE;
         };
        case FileRequestSuccess: {
    //To do: send success page, 상태 코드 들어있음, 그냥 그거 보고 리스폰스 만들어서 송신
          return SUCCESS;
         }
        // FileRequestShouldwait ->block
        default:
          return WAITING;
   }
}