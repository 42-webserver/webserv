#ifndef NOTFOUNDEXCEPTION_HPP
# define NOTFOUNDEXCEPTION_HPP

# include "BadRequestException.hpp"

class NotFoundException : public BadRequestException {
public:
    NotFoundException(): BadRequestException(NOT_FOUND) {}
    const char* what() const throw(){
        return "404 Not Found";
    }
};

#endif