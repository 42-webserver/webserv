#include <Cgi/CgiEnvSetter.hpp>
#include <Http/Utils/RouterUtils.hpp>
CgiEnvSetter* CgiEnvSetter::_instance = NULL;
CgiEnvSetter::CgiEnvSetter(){}
CgiEnvSetter::~CgiEnvSetter(){}
CgiEnvSetter& CgiEnvSetter::getInstance(){
    if (_instance == NULL)
        _instance = new CgiEnvSetter();
    return *_instance;
}

const std::map<std::string, std::string>& CgiEnvSetter::getEnv(ft::shared_ptr<Client> client,
CgiEnvSetter::e_method method, ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> vsm){
    _env.clear();
    _setDefaultEnv(client, channel, vsm);
    if (method == e_get)
        setGetEnv(client, vsm);
    else if (method == e_post)
        setPostAndPutEnv(client, vsm, "POST");
    else if (method == e_put)
        setPostAndPutEnv(client, vsm, "PUT");
    else if (method == e_delete)
        setDeleteEnv(client);
    return _env;
}

void CgiEnvSetter::_setDefaultEnv(ft::shared_ptr<Client> client,
ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> vsm){
    ft::shared_ptr<Socket> socket = ft::static_pointer_cast<Socket>(channel);
    _env["REMOTE_ADDR"] = socket->getClientIp();
    _env["SERVER_PORT"] = vsm->getPort();
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "webserv";
    _env["SERVER_NAME"] = client->getRequest()->getHost();
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

std::string CgiEnvSetter::joinQueries(const std::map<std::string, std::string>& queries) {
    std::stringstream ss;

    for (std::map<std::string, std::string>::const_iterator it = queries.begin(); it != queries.end(); ++it) {
        if (it != queries.begin()) {
            ss << "&";
        }
        ss << it->first << "=" << it->second;
    }

    return ss.str();
}

void CgiEnvSetter::setGetEnv(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm){
    _env["REQUEST_METHOD"] = "GET";
    _env["QUERY_STRING"] = joinQueries(client->getRequest()->getQueries());
    _env["SCRIPT_NAME"] = RouterUtils::findCgiScriptPath(vsm, client->getRequest());
    _env["PATH_INFO"] = RouterUtils::findPathInfo(vsm, client->getRequest());
}

void CgiEnvSetter::setPostAndPutEnv(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm,
std::string method){
    _env["REQUEST_METHOD"] = method;
    _env["QUERY_STRING"] = joinQueries(client->getRequest()->getQueries());
    _env["SCRIPT_NAME"] = RouterUtils::findCgiScriptPath(vsm, client->getRequest());
    _env["PATH_INFO"] = RouterUtils::findPathInfo(vsm, client->getRequest());
    _env["CONTENT_LENGTH"] = client->getRequest()->getHeader("Content-Length");
    _env["CONTENT_TYPE"] = client->getRequest()->getHeader("Content-Type");
}

void CgiEnvSetter::setDeleteEnv(ft::shared_ptr<Client> client){
    _env["REQUEST_METHOD"] = "DELETE";
    _env["QUERY_STRING"] = joinQueries(client->getRequest()->getQueries());
    _env["SCRIPT_NAME"] = client->getRequest()->getUri();
    _env["PATH_INFO"] = client->getRequest()->getUri();
}