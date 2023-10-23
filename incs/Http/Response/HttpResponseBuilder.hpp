#ifndef HTTPRESPONSEBUILDER_HPP
# define HTTPRESPONSEBUILDER_HPP

#include <iostream>
#include "../Utils/HttpStatus.hpp"
#include "../../../libs/Library/Optional.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include <Server/VirtualServerManager.hpp>
#include <vector>
#include <sstream>

class HttpRequest;
class Client;
class DirectoryException: public std::exception
{
public:
    DirectoryException(void);
    virtual ~DirectoryException(void) throw();
    virtual const char* what() const throw();
};

class ContentLength
{
public:
    typedef enum{
        e_chunked_header,
        e_content_length_header
    }   e_content_length_header_type;
private:
    const std::string _transferEncodingHeader;
    int _contentLength;
    e_content_length_header_type _contentLengthHeaderType;
public:
    //for ContentLength::e_content_length_header
    ContentLength(int cotentLength);
    //for ContentLength::e_chunked_header
    ContentLength(void);
    std::string getContentLengthHeader(void);
};
class HttpResponseBuilder
{
private:
    ft::Optional<HttpStatusCode> _statusCode;
    const std::string _serverHeader;
    const std::string _dateHeader;
    std::string _connectionHeader;
    const std::string _KeepAliveHeader;
    ft::Optional<ContentLength> _contentLength;
    ft::shared_ptr<Client> _client;
public:
    HttpResponseBuilder(ft::shared_ptr<Client> client);
    virtual ~HttpResponseBuilder(void);
protected:
    void _setStatusCode(const HttpStatusCode &statusCode);
    void _allocContentLength(ContentLength::e_content_length_header_type contentLengthHeaderType,
                             int contentLength);
    void _buildDefaultResponseHeader(std::vector<char> &buffer);
    ft::shared_ptr<Client> getClient(void);
    static  std::string _makeContentTypeHeader(ft::shared_ptr<HttpRequest> request, std::string indexingPath);
public:
    virtual void buildResponseHeader(std::vector<char> &buffer) = 0;
private:
    static std::string _makeDateHeader(void);
    HttpResponseBuilder(const HttpResponseBuilder &ref);
    HttpResponseBuilder	&operator=(const HttpResponseBuilder &rhs);
};

#endif