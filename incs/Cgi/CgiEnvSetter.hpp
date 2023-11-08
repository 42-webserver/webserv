#ifndef CGIENVSETTER_HPP
# define CGIENVSETTER_HPP

#include <string>
#include <map>
#include <Client/Client.hpp>
#include <Channel/Channel.hpp>
#include <Server/VirtualServerManager.hpp>

class CharArray {
public:
    CharArray(int size);
    CharArray();
    ~CharArray();
    CharArray(const CharArray& other);
    CharArray& operator=(const CharArray& other);

    char** get();
    char* operator[](int i);
    void insert(const std::string& str);
    int size() const;

private:
    int _size;
    char** _data;
    int _mallocSize;

    void _alloc(size_t size);
    void _dealloc();
};
class EnvpManager {
private:
    std::size_t _size;
    CharArray _envp;

public:
    EnvpManager(const std::map<std::string, std::string>& env);
    ~EnvpManager();
    char** getEnvp();
};

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
    static EnvpManager setEnvAll(const std::map<std::string, std::string>& env);
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