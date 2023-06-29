#ifndef BAD_REQUEST_EXCEPTION_HPP
#define BAD_REQUEST_EXCEPTION_HPP

#include <exception>

class BadRequestException : public std::exception {
    public:
            virtual const char* what() const throw() = 0;
};

#endif

