#ifndef DISCONNECTEXCEPTION_HPP
# define DISCONNECTEXCEPTION_HPP

#include <exception>

class DisconnectionException : public std::exception {
public:
    DisconnectionException() {}
    virtual const char* what() const throw () {
        return "Disconnection";
    }
};

#endif