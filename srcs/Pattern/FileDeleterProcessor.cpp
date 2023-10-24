#include <Pattern/FileDeleterProcessor.hpp>
// To do: implement FileDeleterProcessor
FileDeleterProcessor::FileDeleterProcessor(void) {
}
FileDeleterProcessor::~FileDeleterProcessor(void) {
}
e_pattern_Process_result FileDeleterProcessor::process(ft::shared_ptr
    <VirtualServerManager> vsm,
    ft::shared_ptr<Client> client,
    ft::shared_ptr<Channel> channel) {
    (void)channel;
  FileManager& fileManager = FileManager::getInstance();
  e_FileRequestType type;
   try { 
      //basic Handle All RequireMent
      _commandBuildHeaderTo(
        ft::shared_ptr<HttpResponseBuilder>(new DeleteResponseBuilder(client, vsm)),
        client
      );

      std::string path = RouterUtils::findPath(vsm, client->getRequest());
      type = fileManager.requestFileDelete(path);
   }
   catch (std::exception& e) {
       //log error
       throw ;
   }

  //Todo: check this
   switch (type)
   {
        case FileRequestSuccess: {
    //To do: send success page, 상태 코드 들어있음, 그냥 그거 보고 리스폰스 만들어서 송신
          return SUCCESS;
         }
        // FileRequestShouldwait ->block
        default:
          return WAITING;
   }
}

e_pattern_Process_result FileDeleterProcessor::querryCanSending(ft::shared_ptr
    <VirtualServerManager> virtualServerManager,
    ft::shared_ptr<Client> client) {
  FileManager& fileManager = FileManager::getInstance();
  e_FileRequestType type;
  std::string path = RouterUtils::findPath(virtualServerManager, client->getRequest());
  type = fileManager.requestFileDelete(path);
  switch (type)
  {
      case FileRequestSuccess:
          return SUCCESS;
      break;
      default:
          return WAITING;
  }
}