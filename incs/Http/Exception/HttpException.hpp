#ifndef HTTPEXCEPTION_HPP
#define HTTPEXCEPTION_HPP

#include <exception>
#include "../Utils/HttpStatus.hpp"

class HttpException : public std::exception {
private:
    HttpStatusCode _statusCode;

public:
    HttpException(HttpStatusCode statusCode) : _statusCode(statusCode) {}
    HttpStatusCode getStatusCode(){
        return _statusCode;
    }
    virtual const char* what() const throw(){
        return "Http Exception";
    }
};

#endif