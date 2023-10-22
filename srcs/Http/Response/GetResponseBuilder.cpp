#include <Http/Response/GetResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>



GetResponseBuilder::GetResponseBuilder(ft::shared_ptr<Client> client, 
ft::shared_ptr<VirtualServerManager> vsm) : HttpResponseBuilder(client), _vsm(vsm) {
}

GetResponseBuilder::~GetResponseBuilder(void) {
}

void GetResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    bool isDirectory = false;
    std::string dirPath;
    std::string dirListing;
    _setStatusCode(OK);
    try {
        _ContentTypeHeader = HttpResponseBuilder::_makeContentTypeHeader(this->getClient()->getRequest());
    } catch (DirectoryException &e) {
        isDirectory = true;
        dirPath = RouterUtils::findPath(this->_vsm, this->getClient()->getRequest());
        dirListing = FileManager::getDirectoryListing(dirPath);
        // _allocContentLength(ContentLength::e_content_length_header, dirListing.size());
        // throw ;
    }
    if (isDirectory == true) {
        _allocContentLength(ContentLength::e_content_length_header, dirListing.size());
    } else {
        std::string filePath = RouterUtils::findPriorityPathWithIndex(this->_vsm, this->getClient()->getRequest());
        _allocContentLength(ContentLength::e_content_length_header, FileManager::getFileSize(filePath));
    }
    _buildDefaultResponseHeader(buffer);

    std::string header = _ContentTypeHeader + "\r\n\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());
}