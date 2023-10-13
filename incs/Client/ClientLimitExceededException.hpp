#ifndef CLIENTLIMITEXCEEDEDEXCEPTION_HPP
# define CLIENTLIMITEXCEEDEDEXCEPTION_HPP

# include <exception>

class ClientLimitExceededException: public std::exception {
    public:
        virtual const char *what() const throw();
};

#endif