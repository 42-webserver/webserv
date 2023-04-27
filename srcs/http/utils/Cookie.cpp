#include "./Cookie.hpp"

Cookie::Cookie()
{
}

Cookie::Cookie(std::string key, std::string value)
{
    this->_key = key;
    this->_value = value;
    this->_maxAge = "18000000";
}

Cookie::Cookie(std::string key, std::string value, std::string maxAge){
    this->_key = key;
    this->_value = value;
    this->_maxAge = maxAge;
}

std::string Cookie::getKey()
{
    return this->_key;
}

std::string Cookie::getValue()
{
    return this->_value;
}

std::string Cookie::getMaxAge()
{
    return this->_maxAge;
}