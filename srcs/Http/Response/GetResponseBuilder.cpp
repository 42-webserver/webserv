#include <Http/Response/GetResponseBuilder.hpp>
#include <FileManager/FileManager/FileManager.hpp>



GetResponseBuilder::GetResponseBuilder(ft::shared_ptr<Client> client, 
ft::shared_ptr<VirtualServerManager> vsm, std::string indexingPath) : HttpResponseBuilder(client), _vsm(vsm), _indexingPath(indexingPath) {
}

GetResponseBuilder::~GetResponseBuilder(void) {
}


void GetResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    _setStatusCode(OK);
    try {
        _ContentTypeHeader = 
        HttpResponseBuilder::_makeContentTypeHeader(this->_indexingPath);
    } catch (DirectoryException &e) {
        throw;
    }

    std::string filePath = RouterUtils::findPriorityPathWithIndex(this->_vsm, this->getClient()->getRequest());
    struct stat fileStat;
    if (FileManager::getFileInfo(filePath, fileStat) == NotExistFile) {
        std::cerr << "GetResponseBuilder::buildResponseHeader: file not found" << std::endl;
        throw NotFoundException();
    }
    _allocContentLength(ContentLength::e_content_length_header, FileManager::getFileSize(filePath));
    _buildDefaultResponseHeader(buffer);

    std::string header = _ContentTypeHeader + "\r\n\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());
}