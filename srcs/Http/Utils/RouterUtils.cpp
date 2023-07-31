#include "../../../incs/Http/Utils/RouterUtils.hpp"

std::string RouterUtils::findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    ft::shared_ptr<LocationElement> locationElement = findLocation(vsm, req);

    std::string root = _findRoot(vsm, req);
    std::string alias = _findAlias(vsm, req);
    return _makePath(root, alias, uri);
}

std::string RouterUtils::findPriorityPathWithIndex(ft::shared_ptr<VirtualServerManager> vsm, 
        ft::shared_ptr<HttpRequest> req)throw (NotFoundException){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    std::string root = _findRoot(vsm, req);
    std::string alias = _findAlias(vsm, req);
    std::string path = _makePath(root, alias, uri);
    path = _findIndex(vsm, req, path);
}

ft::shared_ptr<LocationElement> RouterUtils::findLocation(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    VirtualServerManager::VirtualServerPtr targetServer = vsm->findVirtualServer(host);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
    ServerElement::ElementPtr locationTrie = it->second;

    ft::shared_ptr<LocationTrieElement> locationTrieElement = ft::static_pointer_cast<LocationTrieElement>(locationTrie);
    ft::shared_ptr<LocationElement> locationElement = locationTrieElement->longestPrefixSearch(uri);
    return locationElement;
}

int RouterUtils::findMaxBodySize(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    VirtualServerManager::VirtualServerPtr targetServer = vsm->findVirtualServer(host);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::CLIENT_MAX_BODY_SIZE);
    if (it == serverElement.end())
        return INT_MAX;
    return ft::static_pointer_cast<ClientMaxBodySizeElement>(it->second)->getNum();
}

std::string RouterUtils::_makePath(std::string &root, std::string &alias, std::string &uri){
    std::string path;
    if(root.empty() && alias.empty())
        path = uri;
    else if(!root.empty())
        path = root + uri;
    else if(!alias.empty())
        path = alias + uri;
    return path;
}

std::string RouterUtils::_findIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req, std::string path){
    if (!FileUploader::isDirectory(path))
        return path;
    ft::shared_ptr<LocationElement> locationElement = findLocation(vsm, req);
    //if location is exist
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::INDEX);
    if (it == locationElement->end()){
       if (FileUploader::isFileExists(path))
            return path;
        throw NotFoundException();
    }
    ft::shared_ptr<ConfigElement> indexConfElem = it->second;
    ft::shared_ptr<IndexElement> indexElem = ft::static_pointer_cast<IndexElement>(indexConfElem);
    std::vector<std::string> indexList = indexElem->getUris();
    for (std::vector<std::string>::iterator it = indexList.begin(); it != indexList.end(); it++){
        std::string indexPath = path + "/" + *it;
        if (FileUploader::isFileExists(indexPath))
            return indexPath;
    }
    throw NotFoundException();

    //if location not exist
    ft::shared_ptr<VirtualServer> targetServer;
    try{
        targetServer = vsm->findVirtualServer(req->getHost());
    } catch (VirtualServerManager::InvalidHostFormatException &e){
        targetServer = vsm->getDefaultVirtualServer();
    }
    ServerElement server_element = targetServer->getServerElement();
    ServerElement::iterator it2 = server_element.find(ServerElement::KEY::INDEX);
    if (it2 == server_element.end()){
        if (FileUploader::isFileExists(path))
            return path;
        throw NotFoundException();
    }
    ft::shared_ptr<ConfigElement> indexConfElem2 = it2->second;
    ft::shared_ptr<IndexElement> indexElem2 = ft::static_pointer_cast<IndexElement>(indexConfElem2);
    std::vector<std::string> indexList2 = indexElem2->getUris();
    for (std::vector<std::string>::iterator it = indexList2.begin(); it != indexList2.end(); it++){
        std::string indexPath = path + "/" + *it;
        if (FileUploader::isFileExists(indexPath))
            return indexPath;
    }
    throw NotFoundException();
}

std::string RouterUtils::_findRoot(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string root;
    ft::shared_ptr<VirtualServer> targetServer;
    try{
        targetServer = vsm->findVirtualServer(req->getHost());
    } catch (VirtualServerManager::InvalidHostFormatException &e){
        targetServer = vsm->getDefaultVirtualServer();
    }
    ServerElement server_element = targetServer->getServerElement();
    ServerElement::iterator it = server_element.find(ServerElement::KEY::ROOT);
    if (it != server_element.end()){
        ft::shared_ptr<ConfigElement> rootConfElem = it->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }

    ft::shared_ptr<LocationElement>  locationElement = findLocation(vsm, req);
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ROOT);
    if(it2 != locationElement->end())
    {
        ft::shared_ptr<ConfigElement> rootConfElem = it2->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    return root;
}

std::string RouterUtils::_findAlias(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string alias;
    ft::shared_ptr<LocationElement>  locationElement = findLocation(vsm, req);
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ALIAS);
    if(it2 != locationElement->end())
    {
        ft::shared_ptr<ConfigElement> rootConfElem = it2->second;
        alias = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    return alias;
}