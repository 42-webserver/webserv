#ifndef CGIENVSETTER_HPP
# define CGIENVSETTER_HPP

#include <string>
#include <map>
#include <Client/Client.hpp>
#include <Channel/Channel.hpp>
#include <Server/VirtualServerManager.hpp>
class CgiEnvSetter {
private:
    std::map<std::string, std::string> _env;
    static CgiEnvSetter* _instance;

private:
    void _setDefaultEnv(ft::shared_ptr<Client> client, ft::shared_ptr<Channel> channel,
    ft::shared_ptr<VirtualServerManager> vsm);
    std::string joinQueries(const std::map<std::string, std::string>& queries);
private:
    void setGetEnv();
    void setPostAndPutEnv(ft::shared_ptr<Client> client, std::string method);
    void setDeleteEnv();
 
public:
    static void setEnvAll(const std::map<std::string, std::string>& env);
    const std::map<std::string, std::string>& getEnv(ft::shared_ptr<Client> client,
    ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> vsm);



private:
    CgiEnvSetter(const CgiEnvSetter& other);
    CgiEnvSetter& operator=(const CgiEnvSetter& other);
    ~CgiEnvSetter();
    CgiEnvSetter();
public:
    static CgiEnvSetter& getInstance();
};


#endif