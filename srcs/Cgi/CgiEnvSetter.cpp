#include <Cgi/CgiEnvSetter.hpp>
#include <Http/Utils/RouterUtils.hpp>
#include <iostream>

CharArray::CharArray(int size) : _size(size), _data(new char*[size + 1]), _mallocSize(0) {
    std::fill_n(_data, _size + 1, static_cast<char*>(0)); // C++98-compliant NULL initialization.
}

CharArray::CharArray() : _size(0), _data(new char*[1]), _mallocSize(0) {
    _data[0] = static_cast<char*>(0); // C++98-compliant NULL initialization.
}


CharArray::~CharArray() {
    _dealloc();
}

CharArray::CharArray(const CharArray& other) : _size(other._size), _data(new char*[other._size + 1]), _mallocSize(other._mallocSize) {
    std::fill_n(_data, _size + 1, static_cast<char*>(0)); // C++98-compliant NULL initialization.
    for (int i = 0; i < _mallocSize; ++i) {
        _data[i] = new char[std::strlen(other._data[i]) + 1];
        std::strcpy(_data[i], other._data[i]);
    }
}

CharArray& CharArray::operator=(const CharArray& other) {
    if (this != &other) {
        _dealloc(); // Deallocate current data.

        _size = other._size;
        _mallocSize = other._mallocSize;
        _data = new char*[_size + 1];
        std::fill_n(_data, _size + 1, static_cast<char*>(0)); // C++98-compliant NULL initialization.
        for (int i = 0; i < _mallocSize; ++i) {
            _alloc(strlen(other._data[i]) + 1);
            std::strcpy(_data[i], other._data[i]);
        }
    }
    return *this;
}

char** CharArray::get() {
    return _data;
}

char* CharArray::operator[](int i) {
    return _data[i];
}


//0 1 2 3
//0 1 2 3
void CharArray::_alloc(size_t size) {
    if (_mallocSize >= _size + 1) {  // Check if we have reached or exceeded capacity
        throw std::length_error("CharArray is at its capacity or invalid allocation attempted."
        + std::to_string(_size));
    }
    _data[_mallocSize] = new char[size];
    std::fill_n(_data[_mallocSize], size, 0); // Fill the allocated memory with 0 (null-terminators for strings).
    ++_mallocSize;
    if (_mallocSize == _size + 1) {
        _data[_mallocSize] = new char(1);
        _data[_mallocSize][0] = '\0';
        ++_mallocSize;
    }
}

void CharArray::insert(const std::string& str) {
    _alloc(str.size() + 1);  // Allocate space for new string plus null-terminator.
    std::strcpy(_data[_mallocSize - 1], str.c_str());
    // if (_ // Copy the string into the newly allocated space.
}

int CharArray::size() const {
    return _mallocSize; // Return the number of strings, not the size of the array.
}

void CharArray::_dealloc() {
    for (int i = 0; i < _mallocSize; ++i) {
        delete[] _data[i];
    }
    delete[] _data;
}


//EnvpManager
EnvpManager::EnvpManager(const std::map<std::string, std::string>& env) :
_size(env.size()), _envp(_size) {

        for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it) {
            std::string envString = it->first + "=" + it->second;
            _envp.insert(envString);
        }
        if (_envp[_envp.size()] != NULL)
            throw std::runtime_error("EnvpManager::EnvpManager::NULL::end");
}

EnvpManager::~EnvpManager() {
}

char **EnvpManager::getEnvp()  {
    return _envp.get();
}

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
    ft::shared_ptr<HttpRequest> request = client->getRequest();
    _env["REMOTE_ADDR"] = socket->getClientIp();
    _env["SERVER_PORT"] = std::to_string(vsm->getPort());
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    _env["SERVER_SOFTWARE"] = "webserv";
    _env["SERVER_NAME"] = request->getHost();
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    //TOdo: check
    _env["QUERY_STRING"] = joinQueries(request->getQueries());
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

EnvpManager CgiEnvSetter::setEnvAll(const std::map<std::string, std::string>& env){
    return EnvpManager(env);
}