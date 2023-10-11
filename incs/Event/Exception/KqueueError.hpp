#ifndef KQUEUEERROR_HPP
# define KQUEUEERROR_HPP

# include <exception>

class KqueueError: public std::exception {
    public:
        virtual const char *what() const throw();
};

#endif