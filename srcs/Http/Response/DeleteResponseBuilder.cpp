#include <Http/Response/DeleteResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>

DeleteResponseBuilder::DeleteResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm) : HttpResponseBuilder(client), _vsm(vsm) {
}

DeleteResponseBuilder::~DeleteResponseBuilder(void) {
}

void DeleteResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    std::string filePath = RouterUtils::findPath(this->_vsm, this->getClient()->getRequest());
    struct stat fileStat;
    e_file_info fileTypeInfo = FileManager::getFileInfo(filePath, fileStat);

    switch (fileTypeInfo)
    {
    case ExistDirectory:
        throw ForbiddenException();
    case ExistFile: 
        _setStatusCode(NO_CONTENT);
        break;
    case NotExistFile: {
        throw NotFoundException();
    default:
        break;
    }
    }
    _allocContentLength(ContentLength::e_content_length_header, 0);
    _buildDefaultResponseHeader(buffer);

   std::string header = "\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());
}