#ifndef SERVERERROREXCEPTION_HPP
#define SERVERERROREXCEPTION_HPP

#include <exception>
#include "../Utils/HttpStatus.hpp"
#include "../Exception/HttpException.hpp"

class ServerErrorException : public HttpException {
public:
    ServerErrorException(HttpStatusCode statusCode) :
    HttpException(statusCode) {}

    const char* what() const throw(){
        return "Internal Server Error";
    }

};

#endif

