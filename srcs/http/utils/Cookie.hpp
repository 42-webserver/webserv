#ifndef COOKIE_HPP
#define COOKIE_HPP
#include <string>

class Cookie{
private:
    std::string _key;
    std::string _value;
    std::string _maxAge;
public:
    Cookie();
    Cookie(std::string key, std::string value);
    Cookie(std::string key, std::string value, std::string maxAge);
    std::string getKey();
    std::string getValue();
    std::string getMaxAge();
};

#endif