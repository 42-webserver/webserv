#ifndef INTERNALSERVERERROREXCEPTION_HPP
#define INTERNALSERVERERROREXCEPTION_HPP

#include <exception>
#include "../Utils/HttpStatus.hpp"
#include "../Exception/HttpException.hpp"

class InternalServerErrorException : public HttpException {
public:
    InternalServerErrorException() :
    HttpException(INTERNAL_SERVER_ERROR) {}

    const char* what() const throw(){
        return "Internal Server Error";
    }

};
#endif