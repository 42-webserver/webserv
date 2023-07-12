#ifndef NOTFOUNDEXCEPTION_HPP
# define NOTFOUNDEXCEPTION_HPP

# include "BadRequestException.hpp"

class NotFoundException : public BadRequestException {
    public:
        virtual const char* what() const throw();
};

#endif