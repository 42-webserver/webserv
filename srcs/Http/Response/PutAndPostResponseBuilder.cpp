#include <Http/Response/PutAndPostResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>

PutAndPostResponseBuilder::PutAndPostResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm) : 
HttpResponseBuilder(client), _vsm(vsm) {
}

PutAndPostResponseBuilder::~PutAndPostResponseBuilder(void) {
}

void PutAndPostResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
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
        _setStatusCode(CREATED);
        _locationHeader = "Location: " + this->getClient()->getRequest()->getUri();
        break;
    default:
        break;
    }
    }
    _allocContentLength(ContentLength::e_content_length_header, 0);
    _buildDefaultResponseHeader(buffer);

   std::string header = _locationHeader.empty() ? "" : _locationHeader + "\r\n";
    header += "\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());
}