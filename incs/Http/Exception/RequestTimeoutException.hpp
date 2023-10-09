#ifndef RequestTimeoutException_HPP
# define RequestTimeoutException_HPP

# include "BadRequestException.hpp"

class RequestTimeoutException : public BadRequestException {
public:
    RequestTimeoutException(): BadRequestException(REQUEST_TIMEOUT) {}
    const char* what() const throw(){
        return "408 Request Timeout";
    }
};

#endif

