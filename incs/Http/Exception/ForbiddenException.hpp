#ifndef FORBIDDENEXCEPTION_HPP
# define FORBIDDENEXCEPTION_HPP

# include "BadRequestException.hpp"

class ForbiddenException : public BadRequestException {
public:
    ForbiddenException(): BadRequestException(FORBIDDEN) {}
    const char* what() const throw(){
        return "403 Forbidden";
    }
};
#endif