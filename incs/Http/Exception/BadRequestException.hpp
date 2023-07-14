#ifndef BAD_REQUEST_EXCEPTION_HPP
#define BAD_REQUEST_EXCEPTION_HPP

#include <exception>
#include "../Utils/HttpStatus.hpp"
#include "../Exception/HttpException.hpp"

class BadRequestException : public HttpException {
public:
    BadRequestException(HttpStatusCode statusCode) : HttpException(statusCode) {}

    const char* what() const throw(){
        return "Bad Request";
    }
};

#endif

