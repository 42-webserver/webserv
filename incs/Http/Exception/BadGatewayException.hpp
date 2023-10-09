#ifndef BADGATEWAYEXCEPTION_HPP
# define BADGATEWAYEXCEPTION_HPP

# include "ServerErrorException.hpp"

class BadGatewayException : public ServerErrorException {
public:
    BadGatewayException(): ServerErrorException(BAD_GATEWAY) {}
    const char* what() const throw(){
        return "502 Bad Gateway";
    }
};

#endif
