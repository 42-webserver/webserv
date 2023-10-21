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
  e_FileRequestType type;
   try { 
      std::string path = RouterUtils::findPriorityPathWithIndex(vsm, client->getRequest());
      type = fileManager.requestFileDelete(path);
   }
   catch (std::exception& e) {
       //log error
       throw ;
   }
    //To do: deleter를 위한 response안에 내용 다 넣어둘 것 필요
  //ex)
// HTTP/1.1 200 OK
// Date: Mon, 23 Oct 2023 05:12:40 GMT
// Server: YourServerName
// Content-Length: 0


   switch (type)
   {
        case FileRequestFail: {
            throw std::runtime_error("FileDeleterProcessor::process : FileRequestFail");
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