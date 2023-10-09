#ifndef METHODNOTALLOWEDEXCEPTION_HPP
# define METHODNOTALLOWEDEXCEPTION_HPP

# include "BadRequestException.hpp"

class MethodNotAllowedException : public BadRequestException {
public:
    MethodNotAllowedException(): BadRequestException(METHOD_NOT_ALLOWED) {}
    const char* what() const throw(){
        return "405 Method Not Allowed";
    }
};

#endif