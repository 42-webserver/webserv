#include "../../../incs/Http/Utils/RouterUtils.hpp"

std::string RouterUtils::findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req){
    std::string uri = req->getUri();
    std::string host = req->getHost();

    ft::shared_ptr<LocationElement> locationElement = findLocation(vsm, req);

    std::string root = _findRoot(locationElement);
    std::string alias = _findAlias(locationElement);
    return _makePath(root, alias, uri);
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



std::string RouterUtils::_findRoot(ft::shared_ptr<LocationElement> locationElement){
    std::string root;
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ROOT);
    if(it2 != locationElement->end())
    {
        ft::shared_ptr<ConfigElement> rootConfElem = it2->second;
        root = ft::static_pointer_cast<RootElement>(rootConfElem)->getPath();
    }
    return root;
}

std::string RouterUtils::_findAlias(ft::shared_ptr<LocationElement> locationElement){
    std::string alias;
    LocationElement::iterator it2 = locationElement->find(LocationElement::KEY::ALIAS);
    if(it2 != locationElement->end())
    {
        ft::shared_ptr<ConfigElement> aliasConfElem = it2->second;
        alias = ft::static_pointer_cast<AliasElement>(aliasConfElem)->getPath();
    }
    return alias;
}