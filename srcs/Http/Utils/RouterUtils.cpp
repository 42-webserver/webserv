#include "../../../incs/Http/Utils/RouterUtils.hpp"
#include <FileManager/FileManager/FileManager.hpp>

// Todo: check path->tri
std::string RouterUtils::findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();

    std::string root = _findRoot(vsm, req);
    std::string alias = _findAlias(vsm, req);
    return _makePath(root, alias, uri);
}

std::string RouterUtils::findPriorityPathWithIndex(ft::shared_ptr<VirtualServerManager> vsm, 
        ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();

    std::string root = _findRoot(vsm, req);
    std::string alias = _findAlias(vsm, req);
    std::string path = _makePath(root, alias, uri);
    return _findIndex(vsm, req, path);
}

std::string RouterUtils::findRedirectUri(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
    ServerElement::ElementPtr locationTrie = it->second;

    ft::shared_ptr<LocationTrieElement> locationTrieElement = ft::static_pointer_cast<LocationTrieElement>(locationTrie);
    ft::shared_ptr<LocationElement> locationElement = locationTrieElement->longestPrefixSearch(uri);
    if (locationElement.get() == NULL)
        return "";
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::RETURN);
    if (it2 == locationElement->end())
        return "";
    ft::shared_ptr<ConfigElement> returnConfElem = it2->second;
    ft::shared_ptr<ReturnElement> returnElem = ft::static_pointer_cast<ReturnElement>(returnConfElem);
    return returnElem->getUri();
}

ft::shared_ptr<LocationElement> RouterUtils::findLocation(ft::shared_ptr<HttpRequest> req,
ft::shared_ptr<VirtualServer> targetServer){
    std::string uri = req->getUri();

    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
    ServerElement::ElementPtr locationTrie = it->second;

    ft::shared_ptr<LocationTrieElement> locationTrieElement = ft::static_pointer_cast<LocationTrieElement>(locationTrie);
    ft::shared_ptr<LocationElement> locationElement = locationTrieElement->longestPrefixSearch(uri);
    return locationElement;
}

int RouterUtils::findMaxBodySize(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string host = req->getHost();

    VirtualServerManager::VirtualServerPtr targetServer = vsm->findVirtualServer(host);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::CLIENT_MAX_BODY_SIZE);
    if (it == serverElement.end())
        return INT_MAX;
    return ft::static_pointer_cast<ClientMaxBodySizeElement>(it->second)->getNum();
}

bool RouterUtils::isCgiRequest(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationElement> locationElement = findLocation(req, targetServer);
    if (locationElement.get() == NULL)
        return false;
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::CGI_PASS);
    if (it == locationElement->end())
        return false;
    return ft::static_pointer_cast<CgiPassElement>(it->second)->getFlag().compare("on") == 0;
}
std::string RouterUtils::findCgiScriptPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string fullPath = findPath(vsm, req);

    size_t pos = fullPath.rfind(".cgi");
    if (pos != std::string::npos) {
        return fullPath.substr(0, pos + 4);
    }
    //TO do: 빈 문자열로할지 throw로 할지
    return "";
}
std::string RouterUtils::findPathInfo(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string fullPath = findPath(vsm, req);
    std::string cgiScriptPath = findCgiScriptPath(vsm, req);

    return fullPath.substr(cgiScriptPath.size());
}

bool RouterUtils::isMethodAllowed(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationElement> locationElement = findLocation(req, targetServer);
    std::string method = req->getMethod();
    if (locationElement.get() == NULL)
        return method.compare(HTTP_METHOD::GET) == 0;
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::ALLOW_METHOD);
    if (it == locationElement->end())
        return method.compare(HTTP_METHOD::GET) == 0;
    ft::shared_ptr<ConfigElement> allowMethodsConfElem = it->second;
    ft::shared_ptr<AllowMethodElement> allowMethodsElem = ft::static_pointer_cast<AllowMethodElement>(allowMethodsConfElem);
    
    if (allowMethodsElem->getFlag() == M_GET && method.compare(HTTP_METHOD::GET) == 0)
        return true;
    if (allowMethodsElem->getFlag() == M_POST && method.compare(HTTP_METHOD::POST) == 0)
        return true;
    if (allowMethodsElem->getFlag() == M_PUT && method.compare(HTTP_METHOD::PUT) == 0)
        return true;
    if (allowMethodsElem->getFlag() == M_DELETE && method.compare(HTTP_METHOD::DELETE) == 0)
        return true;
    return false;
}

bool RouterUtils::isRedirection(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationElement> locationElement = findLocation(req, targetServer);
    if (locationElement.get() == NULL)
        return false;
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::RETURN);
    if (it == locationElement->end())
        return false;
    return true;
}

std::string RouterUtils::_makePath(std::string &root, std::string &alias, std::string &uri){
    std::string path;
    if(root.empty() && alias.empty())
        path = uri;
    else if(!root.empty())
        path = root + "/" + uri;
    else if(!alias.empty()){
        path =  alias + "/" + uri.substr(uri.find_last_of('/') + 1);
    }
    return path;
}

std::string RouterUtils::_findIndexInLocation(std::string path, ft::shared_ptr<LocationElement> locationElement){
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::INDEX);
    if (it == locationElement->end()){
        if (FileManager::isFileExists(path))
            return path;
        else if (FileManager::isFileExists(path + "/index.html"))
            return path + "/index.html";
        throw NotFoundException();
    }
    ft::shared_ptr<ConfigElement> indexConfElem = it->second;
    ft::shared_ptr<IndexElement> indexElem = ft::static_pointer_cast<IndexElement>(indexConfElem);
    std::vector<std::string> indexList = indexElem->getUris();
    for (std::vector<std::string>::iterator it = indexList.begin(); it != indexList.end(); it++){
        std::string indexPath = path + "/" + *it;
        if (FileManager::isFileExists(indexPath))
            return indexPath;
    }
    throw NotFoundException();
}

std::string RouterUtils::_findIndexInServer(std::string path, ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    ft::shared_ptr<VirtualServer> targetServer;
    try{
        targetServer = vsm->findVirtualServer(req->getHost());
    } catch (VirtualServerManager::InvalidHostFormatException &e){
        targetServer = vsm->getDefaultVirtualServer();
    }
    ServerElement server_element = targetServer->getServerElement();
    ServerElement::iterator it2 = server_element.find(ServerElement::KEY::INDEX);
    if (it2 == server_element.end()){
        if (FileManager::isFileExists(path))
            return path;
        else if (FileManager::isFileExists(path + "/index.html"))
            return path + "/index.html";
        throw NotFoundException();
    }
    ft::shared_ptr<ConfigElement> indexConfElem2 = it2->second;
    ft::shared_ptr<IndexElement> indexElem2 = ft::static_pointer_cast<IndexElement>(indexConfElem2);
    std::vector<std::string> indexList2 = indexElem2->getUris();
    for (std::vector<std::string>::iterator it = indexList2.begin(); it != indexList2.end(); it++){
        std::string indexPath = path + "/" + *it;
        if (FileManager::isFileExists(indexPath))
            return indexPath;
    }
    throw NotFoundException();
}

std::string RouterUtils::_findIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req, std::string path){
    if (!FileManager::isDirectory(path))
            return path;
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationElement> locationElement = findLocation(req, targetServer);
    if (locationElement.get() != NULL){
        LocationElement::iterator it = locationElement->find(LocationElement::KEY::AUTOINDEX);
        if (it != locationElement->end()){
            ft::shared_ptr<AutoIndexElement> autoIndexElem = ft::static_pointer_cast<AutoIndexElement>(it->second);
            if (autoIndexElem->getFlag())
                return path;
        }
        return _findIndexInLocation(path, locationElement);
    }
    else
        return _findIndexInServer(path, vsm, req);
}

ft::shared_ptr<VirtualServer> RouterUtils::_findVirtualServer(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    ft::shared_ptr<VirtualServer> targetServer;
    try{
        targetServer = vsm->findVirtualServer(req->getHost());
    } catch (VirtualServerManager::InvalidHostFormatException &e){
        targetServer = vsm->getDefaultVirtualServer();
    }
    return targetServer;
}

std::string RouterUtils::_findRoot(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string root;
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ServerElement server_element = targetServer->getServerElement();
    ServerElement::iterator it = server_element.find(ServerElement::KEY::ROOT);
    if (it != server_element.end()){
        ft::shared_ptr<ConfigElement> rootConfElem = it->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }

    ft::shared_ptr<LocationElement>  locationElement = findLocation(req, targetServer);
    if(locationElement.get() == NULL)
        return root;
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ROOT);
    if(it2 != locationElement->end()){
        ft::shared_ptr<ConfigElement> rootConfElem = it2->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    return root;
}

std::string RouterUtils::_findAlias(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string alias;
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationElement>  locationElement = findLocation(req, targetServer);
    if(locationElement.get() == NULL)
        return alias;
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ALIAS);
    if(it2 != locationElement->end()){
        ft::shared_ptr<ConfigElement> rootConfElem = it2->second;
        alias = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    return alias;
}