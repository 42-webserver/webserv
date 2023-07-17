#ifndef COOKIE_HPP
#define COOKIE_HPP
#include <string>
#include <iostream>
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
	friend std::ostream &operator<<(std::ostream & os, const Cookie & cookie){
		os << cookie._key << "=" << cookie._value;
		if (cookie._maxAge != "")
			os << "; Max-Age=" << cookie._maxAge;
		return os;
	}
};

#endif