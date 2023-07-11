#include "../../../incs/Http/Utils/RouterUtils.hpp"

std::string RouterUtils::findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req)
{
    std::string uri = req->getUri();
    std::string host = req->getHost();
    VirtualServerManager::VirtualServerPtr targetServer = vsm->findVirtualServer(host);
    ServerElement serverElement = targetServer->getServerElement();
    ServerElement::iterator it = serverElement.find(ServerElement::KEY::LOCATION_TRIE);
    ServerElement::ElementPtr locationTrie = it->second;

}