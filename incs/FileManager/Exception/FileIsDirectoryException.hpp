#ifndef FILEISDIRECTORYEXCEPTION_HPP
# define FILEISDIRECTORYEXCEPTION_HPP

# include <exception>
# include <string>

class FileIsDirectoryException : public std::exception {
private:
    std::string _msg;
    mutable std::string _fullMsg;  
public:
    FileIsDirectoryException(const std::string &msg);
    virtual ~FileIsDirectoryException() throw();

    virtual const char *what() const throw();
};
#endif