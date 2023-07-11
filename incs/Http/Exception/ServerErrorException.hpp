#ifndef SERVERERROREXCEPTION_HPP
#define SERVERERROREXCEPTION_HPP

#include <exception>

class ServerErrorException : public std::exception {
    public:
            virtual const char* what() const throw() = 0;
};

#endif

