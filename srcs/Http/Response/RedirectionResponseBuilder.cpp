#include <Http/Response/RedirectionResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>

RedirectionResponseBuilder::RedirectionResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm) :
HttpResponseBuilder(client), _vsm(vsm) {}

RedirectionResponseBuilder::~RedirectionResponseBuilder(void) {}

void RedirectionResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    std::string filePath = RouterUtils::findPath(this->_vsm, this->getClient()->getRequest());
    struct stat fileStat;  
    e_file_info fileTypeInfo = FileManager::getFileInfo(filePath, fileStat);
    std::string locationHeader;
    if (fileTypeInfo == ExistDirectory) {
        _setStatusCode(MOVED_PERMANENTLY);
        locationHeader = "Location: " + this->getClient()->getRequest()->getUri() + "/";
    }
    else {
        ft::shared_ptr<ReturnElement> returnElement = RouterUtils::findRedirectUri(this->_vsm, this->getClient()->getRequest());
        if (returnElement.get() == NULL) {
            throw NotFoundException();
        }
        _setStatusCode((HttpStatusCode)returnElement->getCode());
        locationHeader = "Location: " + returnElement->getUri();
    }
    _buildEssentialResponseHeader(buffer);

    std::string header = locationHeader + "\r\n\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());
}