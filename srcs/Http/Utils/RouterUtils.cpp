#include "../../../incs/Http/Utils/RouterUtils.hpp"
#include <FileManager/FileManager/FileManager.hpp>

Alias::Alias(std::string location, std::string alias) : _location(location), _alias(alias){
}

Alias::~Alias(void){
}

Alias::Alias(void) : _location(""), _alias(""){
}

Alias::Alias(const Alias& other) : _location(other._location), _alias(other._alias){
}

Alias& Alias::operator=(const Alias& other){
    if (this == &other)
        return *this;
    this->_location = other._location;
    this->_alias = other._alias;
    return *this;
}

const std::string& Alias::getLocation(void) const{
    return this->_location;
}

const std::string& Alias::getAlias(void) const{
    return this->_alias;
}

bool Alias::empty(void) const{
    return this->_location.empty() && this->_alias.empty();
}

// Todo: check path->tri
std::string RouterUtils::findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();

    std::string root = _findRoot(vsm, req);
    Alias alias = _findAlias(vsm, req);
    return _makePath(root, alias, uri);
}

// 일단 bool isRedirection을 고쳐서 이거 불를떄 디렉토리인데 /안붙은거 안오게 했음
std::string RouterUtils::findPriorityPathWithIndex(ft::shared_ptr<VirtualServerManager> vsm, 
        ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();

    std::cerr << "here? : " << uri << std::endl;
    std::string root = _findRoot(vsm, req);
    std::cerr << "root : " << root << std::endl;
    Alias alias = _findAlias(vsm, req);
    std::string path = _makePath(root, alias, uri);
    std::cerr << "path : " << path << std::endl;
    if (path[path.size() - 1] == '/') {
        return _findIndex(vsm, req, path);
    }
    else
        return path;
}

ft::shared_ptr<LocationTrieElement> RouterUtils::_findLocationTrieElement(ft::shared_ptr<VirtualServer> targetServer){
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
    if (it == serverElement.end())
        return ft::shared_ptr<LocationTrieElement>();
    ServerElement::ElementPtr locationTrie = it->second;

    ft::shared_ptr<LocationTrieElement> locationTrieElement = ft::static_pointer_cast<LocationTrieElement>(locationTrie);
    return locationTrieElement;
}

// ft::shared_ptr<LocationElement> RouterUtils::_findLocation(ft::shared_ptr<HttpRequest> req,
// ft::shared_ptr<VirtualServer> targetServer){
//     std::string uri = req->getUri();

//     ServerElement serverElement = targetServer->getServerElement();
//     ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
//     ServerElement::ElementPtr locationTrie = it->second;

//     ft::shared_ptr<LocationTrieElement> locationTrieElement = ft::static_pointer_cast<LocationTrieElement>(locationTrie);
//     ft::shared_ptr<LocationElement> locationElement = locationTrieElement->longestPrefixSearch(uri);
//     return locationElement;
// }

ft::shared_ptr<LocationElement> RouterUtils::_findLocation(ft::shared_ptr<LocationTrieElement> locationTrieElement,
std::string uri){
    ft::shared_ptr<LocationElement> locationElement = locationTrieElement->longestPrefixSearch(uri);
    return locationElement;
}

std::string RouterUtils::_findLocationString(ft::shared_ptr<LocationTrieElement> locationTrieElement,
std::string uri){
    return locationTrieElement->longestPrefixString(uri);
}

ssize_t RouterUtils::findMaxBodySize(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string host = req->getHost();

    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::CLIENT_MAX_BODY_SIZE);
    if (it == serverElement.end())
        //Todo: check this 80mb
        return 1024 * 1024 * 50;
    std::cerr << "findMaxBodySize : " << ft::static_pointer_cast<ClientMaxBodySizeElement>(it->second)->getNum() << std::endl;
    // exit(1);
    return ft::static_pointer_cast<ClientMaxBodySizeElement>(it->second)->getNum();
}

bool RouterUtils::isCgiRequest(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    if (locationTrieElement.get() == NULL)
        return false;
    ft::shared_ptr<LocationElement> locationElement = _findLocation(locationTrieElement, req->getUri());
    if (locationElement.get() == NULL)
        return false;
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::CGI_PASS);
    if (it == locationElement->end())
        return false;
    return ft::static_pointer_cast<CgiPassElement>(it->second)->getFlag().compare("on") == 0;
}

std::string RouterUtils::findCgiScriptPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string fullPath = findPath(vsm, req);

    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    if (locationTrieElement.get() == NULL)
        return "";
    
    std::string locationString = _findLocationString(locationTrieElement, req->getUri());

    // locationString과 일치하는 부분부터 시작
    size_t startPos = fullPath.find(locationString) + locationString.size();

    // '.'을 찾기 시작
    startPos = fullPath.find(".", startPos);
    if (startPos != std::string::npos) {
        size_t endPos = fullPath.find("/", startPos);
        if (endPos == std::string::npos) {
            // '/'가 없으면 문자열 끝까지
            return fullPath.substr(startPos);
        } else {
            return fullPath.substr(startPos, endPos - startPos);
        }
    }

    throw std::runtime_error("CGI Script Path Not Found");
    return "";
}

std::string RouterUtils::findPathInfo(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string fullPath = findPath(vsm, req);
    std::string cgiScriptPath = findCgiScriptPath(vsm, req);

    return fullPath.substr(cgiScriptPath.size());
}

bool RouterUtils::isMethodAllowed(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    ft::shared_ptr<LocationElement> locationElement = ft::shared_ptr<LocationElement>();
    
    if (locationTrieElement.get() != NULL)
        locationElement = _findLocation(locationTrieElement, req->getUri());
    
    std::string method = req->getMethod();
    //get Default
    if (locationElement.get() == NULL)
        return method.compare(HTTP_METHOD::GET) == 0;
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::ALLOW_METHOD);
    if (it == locationElement->end())
        return method.compare(HTTP_METHOD::GET) == 0;
    ft::shared_ptr<ConfigElement> allowMethodsConfElem = it->second;
    ft::shared_ptr<AllowMethodElement> allowMethodsElem = ft::static_pointer_cast<AllowMethodElement>(allowMethodsConfElem);
    if (allowMethodsElem.get() == NULL)
        return method.compare(HTTP_METHOD::GET) == 0;
        // std::cerr << "allowMethodsElem->getFlag() : " << allowMethodsElem->getFlag() << std::endl;
    // if (allowMethodsElem->getFlag() == M_GET && method.compare(HTTP_METHOD::GET) == 0)
    //     return true;
    // if (allowMethodsElem->getFlag() == M_POST && method.compare(HTTP_METHOD::POST) == 0)
    //     return true;
    // if (allowMethodsElem->getFlag() == M_PUT && method.compare(HTTP_METHOD::PUT) == 0)
    //     return true;
    // if (allowMethodsElem->getFlag() == M_DELETE && method.compare(HTTP_METHOD::DELETE) == 0)
    //     return true;
    if (allowMethodsElem->isTurnOnMethod(method))
        return true;
    return false;
}

bool RouterUtils::isRedirection(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    // /안끝나는데  디렉토리면 redirect 
    // Todo : 일단 뗌질로 해놓음
    std::cerr << "is in uri " << uri << std::endl;
    if (uri[uri.size() - 1] != '/') {
        std::cerr << "is in uri " << uri << std::endl;
        req->setUri(uri + "/");
        std::cerr << req->getUri() << std::endl;
        std::string fullPath = findPath(vsm, req);
        std::cerr << "is in fullpath IN REDIRECT : " << fullPath << std::endl;
        req->setUri(uri.substr(0, uri.size()));
        if (FileManager::isDirectory(fullPath))
            return true;
    }
    std::cerr << "is in uri going to find location : " << uri << std::endl;
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    if (locationTrieElement.get() == NULL)
        return false;
    ft::shared_ptr<LocationElement> locationElement = _findLocation(locationTrieElement, req->getUri());
    if (locationElement.get() == NULL)
        return false;
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::RETURN);
    if (it == locationElement->end())
        return false;
    return true;
}

std::string RouterUtils::_makePath(const std::string &root, const Alias &alias, const std::string &uri){
    std::string path;
    if(root.empty() && alias.empty())
        path = uri;
    else if(!alias.empty()){
        const std::string aliasLocation = alias.getLocation();
        std::size_t locationPos = uri.find(aliasLocation);
        if (locationPos != std::string::npos) {
            path = alias.getAlias() + uri.substr(locationPos + aliasLocation.size());
        }
        else {
           //logerrr
           throw std::runtime_error("Alias Logic Error");
        }
        std::cerr << path << std::endl;
    }
    else if(!root.empty()){
        path = root + uri;
    }
    return path;
}

std::string RouterUtils::_findIndexInLocation(std::string path, ft::shared_ptr<LocationElement> locationElement,
ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm){
    LocationElement::iterator it = locationElement->find(LocationElement::KEY::INDEX);
    if (it == locationElement->end()){
        LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::AUTOINDEX);
       if (it2 != locationElement->end()){
            ft::shared_ptr<ConfigElement> autoIndexConfElem = it2->second;
            ft::shared_ptr<AutoIndexElement> autoIndexElem = ft::static_pointer_cast<AutoIndexElement>(autoIndexConfElem);
            if (autoIndexElem->getFlag())
                return path;
            else
                return _findIndexInServer(vsm, req);
        }
        return _findIndexInServer(vsm, req);
    }
    ft::shared_ptr<ConfigElement> indexConfElem = it->second;
    ft::shared_ptr<IndexElement> indexElem = ft::static_pointer_cast<IndexElement>(indexConfElem);
    std::vector<std::string> indexList = indexElem->getUris();
    for (std::vector<std::string>::iterator it = indexList.begin(); it != indexList.end(); it++){
        std::string indexPath = path + *it;
        std::cerr << "find indexPath : " << indexPath << std::endl;
        if (FileManager::isFileExists(indexPath))
            return indexPath;
    }
    return _findIndexInServer(vsm, req);
}

std::string RouterUtils::_findIndexInServer(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string serverPath = _findServerPath(vsm, req);

    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::INDEX);
    if (it == serverElement.end())
        throw NotFoundException();
    ft::shared_ptr<ConfigElement> indexConfElem = it->second;
    ft::shared_ptr<IndexElement> indexElem = ft::static_pointer_cast<IndexElement>(indexConfElem);
    std::vector<std::string> indexList = indexElem->getUris();
    for (std::vector<std::string>::iterator it = indexList.begin(); it != indexList.end(); it++){
        std::string indexPath = serverPath  + *it;
        if (FileManager::isFileExists(indexPath))
            return indexPath;
    }
    throw NotFoundException();
}

std::string RouterUtils::_findIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req, std::string path){
    // if (!FileManager::isDirectory(path))
    //         return path;
    // 여기 왔으면 경로는 일단 무조건 /로 끝남
    // 구현 순서 -> index파일들 다 있는지확인 -> 없으면 autoindex 확인 -> 없으면 404
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    // ft::shared_ptr<LocationElement> locationElement = _findLocation(req, targetServer);
    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    ft::shared_ptr<LocationElement> locationElement = ft::shared_ptr<LocationElement>();
    if (locationTrieElement.get() != NULL)
        std::cerr << "locationTrieElement.get() != NULL" << std::endl;
        locationElement = _findLocation(locationTrieElement, req->getUri());

    try { 
        if (locationElement.get() != NULL){       
            return _findIndexInLocation(path, locationElement, req, vsm);
        }
        else
            return _findIndexInServer(vsm, req);
    }
    catch (NotFoundException &e){
        if (locationElement.get() != NULL){
            LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::AUTOINDEX);
            if (it2 != locationElement->end()){
                ft::shared_ptr<ConfigElement> autoIndexConfElem = it2->second;
                ft::shared_ptr<AutoIndexElement> autoIndexElem = ft::static_pointer_cast<AutoIndexElement>(autoIndexConfElem);
                if (autoIndexElem->getFlag())
                    throw DirectoryException();
            }
        }
        else 
            throw ;
    }
    catch (std::exception &e){
        std::runtime_error("Index Logic Error");
    }
    return "";
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

    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    if(locationTrieElement.get() == NULL)
        return root;
    ft::shared_ptr<LocationElement> locationElement = _findLocation(locationTrieElement, req->getUri());
    if(locationElement.get() == NULL)
        return root;
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ROOT);
    if(it2 != locationElement->end()){
        ft::shared_ptr<ConfigElement> rootConfElem = it2->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    return root;
}

Alias RouterUtils::_findAlias(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string alias;
    std::string LocationString;
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ft::shared_ptr<LocationTrieElement> locationTrieElement = _findLocationTrieElement(targetServer);
    if (locationTrieElement.get() == NULL)
        return Alias();
    ft::shared_ptr<LocationElement> locationElement = _findLocation(locationTrieElement, req->getUri());
    if (locationElement.get() == NULL)
        return Alias();
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ALIAS);
    if (it2 == locationElement->end())
        return Alias();
    ft::shared_ptr<ConfigElement> aliasConfElem = it2->second;
    alias = ft::static_pointer_cast<AliasElement>(aliasConfElem)->getPath();
    LocationString = _findLocationString(locationTrieElement, req->getUri());
    return Alias(LocationString, alias);
}

// Todo: this Should be changed -> 제가 고칠게여 
ft::shared_ptr<ReturnElement> RouterUtils::findRedirectUri(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    std::cerr << "in findRedirectUri : " << uri << std::endl;
    ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
    ServerElement::ElementPtr locationTrie = it->second;

    ft::shared_ptr<LocationTrieElement> locationTrieElement = ft::static_pointer_cast<LocationTrieElement>(locationTrie);
    ft::shared_ptr<LocationElement> locationElement = locationTrieElement->longestPrefixSearch(uri);
    if (locationElement.get() == NULL)
        return ft::shared_ptr<ReturnElement>();
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::RETURN);
    if (it2 == locationElement->end())
        return ft::shared_ptr<ReturnElement>();

    ft::shared_ptr<ConfigElement> returnConfElem = it2->second;
    ft::shared_ptr<ReturnElement> returnElem = ft::static_pointer_cast<ReturnElement>(returnConfElem);
    return returnElem;
}

std::string RouterUtils::_findServerPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
   ft::shared_ptr<VirtualServer> targetServer = _findVirtualServer(vsm, req);
    std::string root;
    ServerElement server_element = targetServer->getServerElement();
    ServerElement::iterator it = server_element.find(ServerElement::KEY::ROOT);
    if (it != server_element.end()){
        ft::shared_ptr<ConfigElement> rootConfElem = it->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    // 무조건 뒤에 /끝남 
    return _makePath(root, Alias(), req->getUri());
}