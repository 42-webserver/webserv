#include <Http/Response/GetResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>



GetResponseBuilder::GetResponseBuilder(ft::shared_ptr<Client> client, 
ft::shared_ptr<VirtualServerManager> vsm, std::string indexingPath) : HttpResponseBuilder(client), _vsm(vsm), _indexingPath(indexingPath) {
}

GetResponseBuilder::~GetResponseBuilder(void) {
}


void GetResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    bool isDirectory = false;
    std::string dirPath;
    std::string dirListing;
    _setStatusCode(OK);
    try {
        _ContentTypeHeader = 
        HttpResponseBuilder::_makeContentTypeHeader(this->getClient()->getRequest(), 
        this->_indexingPath);
    } catch (DirectoryException &e) {
        isDirectory = true;
        dirPath = RouterUtils::findPath(this->_vsm, this->getClient()->getRequest());
        dirListing = FileManager::getDirectoryListing(dirPath);
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

    //directory listing
    if (isDirectory == true) {
        buffer.insert(buffer.end(), dirListing.begin(), dirListing.end());
        throw DirectoryException();
    }
}