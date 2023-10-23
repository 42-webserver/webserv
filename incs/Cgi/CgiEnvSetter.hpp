#ifndef CGIENVSETTER_HPP
# define CGIENVSETTER_HPP

#include <string>
#include <map>
#include <Client/Client.hpp>
#include <Channel/Channel.hpp>
#include <Server/VirtualServerManager.hpp>
class CgiEnvSetter {
public:
    typedef enum{
        e_get,
        e_post,
        e_put,
        e_delete,
        e_method_size
    }   e_method;
private:
    std::map<std::string, std::string> _env;
    static CgiEnvSetter* _instance;
public:
    const std::map<std::string, std::string>& getEnv(ft::shared_ptr<Client> client,
    e_method method, ft::shared_ptr<Channel> channel, ft::shared_ptr<VirtualServerManager> vsm);
private:
    void _setDefaultEnv(ft::shared_ptr<Client> client, ft::shared_ptr<Channel> channel,
    ft::shared_ptr<VirtualServerManager> vsm);
    std::string joinQueries(const std::map<std::string, std::string>& queries);
public:
    void setGetEnv(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm);
    void setPostAndPutEnv(ft::shared_ptr<Client> client, ft::shared_ptr<VirtualServerManager> vsm,std::string method);
    void setDeleteEnv(ft::shared_ptr<Client> client);
    static void execute(const std::map<std::string, std::string>& env);
private:
    CgiEnvSetter(const CgiEnvSetter& other);
    CgiEnvSetter& operator=(const CgiEnvSetter& other);
    ~CgiEnvSetter();
    CgiEnvSetter();
public:
    static CgiEnvSetter& getInstance();
};


#endif