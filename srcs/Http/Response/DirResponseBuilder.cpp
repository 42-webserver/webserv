#include <Http/Response/DirResponseBuilder.hpp>
#include <Http/Utils/RouterUtils.hpp>
#include <FileManager/FileManager/FileManager.hpp>

DirResponseBuilder::DirResponseBuilder(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm) : HttpResponseBuilder(client),   _ContentTypeHeader("Content-Type: text/html\r\n"), _vsm(vsm){
}

DirResponseBuilder::~DirResponseBuilder(void) {
}

void DirResponseBuilder::buildResponseHeader(std::vector<char> &buffer) {
    std::string dirPath = RouterUtils::findPath(this->_vsm, this->getClient()->getRequest());
    std::string dirListing = FileManager::getDirectoryListing(dirPath);
    _setStatusCode(OK);
    _allocContentLength(ContentLength::e_content_length_header, dirListing.size());
    _buildDefaultResponseHeader(buffer);

    std::string header = _ContentTypeHeader + "\r\n\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());
    buffer.insert(buffer.end(), dirListing.begin(), dirListing.end());
}