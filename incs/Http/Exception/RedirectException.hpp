#ifndef REDIRECTEXCEPTION_HPP
# define REDIRECTEXCEPTION_HPP

# include "HttpException.hpp"

class RedirectException : public HttpException {
public:
    RedirectException(): HttpException(MOVED_PERMANENTLY) {}
    const char* what() const throw(){
        return "301 Moved Permanently";
    }
};

#endif