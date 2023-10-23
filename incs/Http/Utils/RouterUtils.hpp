#ifndef ROUTERUTILS_HPP
# define ROUTERUTILS_HPP
#include "../../Config/ServerElement.hpp"
#include "../../Config/LocationTrieElement.hpp"
#include "../../Config/IndexElement.hpp"
#include "../../Config/LocationElement.hpp"
#include "../../Config/RootElement.hpp"
#include "../../Config/AllowMethodElement.hpp"
#include "../../Config/AliasElement.hpp"
#include "../../Config/AutoIndexElement.hpp"
#include "../../Config/ReturnElement.hpp"
#include "../../Server/VirtualServer.hpp"
#include "../../Server/VirtualServerManager.hpp"
#include "../Request/HttpRequest.hpp"
#include "../../Config/ClientMaxBodySizeElement.hpp"
#include "../../Config/CgiPassElement.hpp"
// #include "../Utils/FileUploader.hpp"
#include "../../Config/ReturnElement.hpp"
#include "../Utils/HttpMethod.hpp"
#include "../Exception/NotFoundException.hpp"

class Alias{
private:
    std::string _location;
    std::string _alias;
public:
    Alias(void);
    Alias(std::string location, std::string alias);
    ~Alias(void);
    Alias(const Alias& other);
    Alias& operator=(const Alias& other);
    const std::string& getLocation(void) const;
    const std::string& getAlias(void) const;
    bool empty(void) const;
};

class RouterUtils {
public:
    static std::string	findPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string	findPriorityPathWithIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string  findRedirectUri(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static int          findMaxBodySize(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static bool         isCgiRequest(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);

    static bool         isMethodAllowed(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static bool         isRedirection(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string findCgiScriptPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);

    static std::string findPathInfo(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
private:
    static ft::shared_ptr<LocationElement> _findLocation(ft::shared_ptr<LocationTrieElement> locationTrieElement,
    std::string uri);
    static std::string _findLocationString(ft::shared_ptr<LocationTrieElement> locationTrieElement,
    std::string uri);
    static std::string	_findRoot(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static Alias	    _findAlias(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static std::string  _makePath(const std::string& root, const Alias& alias, const std::string& uri);
    static std::string _findIndex(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req, std::string path);
    static std::string _findIndexInLocation(std::string path, ft::shared_ptr<LocationElement> locationElement,
    ft::shared_ptr<HttpRequest> req, ft::shared_ptr<VirtualServerManager> vsm);
    static std::string _findIndexInServer(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static ft::shared_ptr<VirtualServer> _findVirtualServer(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
    static ft::shared_ptr<LocationTrieElement> _findLocationTrieElement(ft::shared_ptr<VirtualServer> targetServer);
    static std::string _findServerPath(ft::shared_ptr<VirtualServerManager> vsm, ft::shared_ptr<HttpRequest> req);
};  

#endif