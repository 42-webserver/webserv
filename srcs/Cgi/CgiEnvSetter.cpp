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
    ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> vsm){
    _env.clear();
    _setDefaultEnv(client, channel, vsm);
    std::string method = client->getRequest()->getMethod();
    if (method == "GET")
        setGetEnv();
    else if (method == "POST")
        setPostAndPutEnv(client, "POST");
    else if (method == "PUT")
        setPostAndPutEnv(client, "PUT");
    else if (method == "DELETE")
        setDeleteEnv();
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
    //TOdo: check
    _env["QUERY_STRING"] = joinQueries(client->getRequest()->getQueries());
    _env["SCRIPT_NAME"] = RouterUtils::findCgiScriptPath(vsm, client->getRequest());
    _env["PATH_INFO"] = RouterUtils::findPathInfo(vsm, client->getRequest());
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

void CgiEnvSetter::setGetEnv(){
    _env["REQUEST_METHOD"] = "GET";

}

void CgiEnvSetter::setPostAndPutEnv(ft::shared_ptr<Client> client,
std::string method){
    _env["REQUEST_METHOD"] = method;
    _env["CONTENT_LENGTH"] = client->getRequest()->getHeader("Content-Length");
    _env["CONTENT_TYPE"] = client->getRequest()->getHeader("Content-Type");
}

void CgiEnvSetter::setDeleteEnv(){
    _env["REQUEST_METHOD"] = "DELETE";

}

void CgiEnvSetter::setEnvAll(const std::map<std::string, std::string>& env){
    std::map<std::string, std::string>::const_iterator it = env.begin();
    for (; it != env.end(); ++it){
       setenv(it->first.c_str(), it->second.c_str(), 1);
    }
}