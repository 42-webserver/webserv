#ifndef GATEWAYTIMEOUTEXCEPTION_HPP
# define GATEWAYTIMEOUTEXCEPTION_HPP

# include "ServerErrorException.hpp"

class GatewayTimeoutException : public ServerErrorException {
public:
    GatewayTimeoutException(): ServerErrorException(GATEWAY_TIMEOUT) {}
    const char* what() const throw(){
        return "504 Gateway Timeout";
    }
};

#endif