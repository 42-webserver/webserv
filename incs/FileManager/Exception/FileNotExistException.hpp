#ifndef FILENOTEXISTEXCEPTION_HPP
# define FILENOTEXISTEXCEPTION_HPP

# include <exception>
# include <string>


class FileNotExistException : public std::exception {
private:
    std::string _msg;
    mutable std::string _fullMsg;  

public:
    FileNotExistException(const std::string &msg);
    virtual ~FileNotExistException() throw();

    virtual const char *what() const throw();
};


#endif