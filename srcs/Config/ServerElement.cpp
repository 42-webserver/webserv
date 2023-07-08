#include "../../incs/Config/ServerElement.hpp"
#include "../../incs/Config/RootElement.hpp"
#include "../../incs/Config/IndexElement.hpp"
#include "../../incs/Config/ErrorPageElement.hpp"
#include "../../incs/Config/ClientMaxBodySizeElement.hpp"
#include "../../incs/Config/LocationTrieElement.hpp"
#include "../../incs/Log/Logger.hpp"

const ServerElement::KeyMap	ServerElement::_key_map = ServerElement::_initKeyMap();
ServerElement::KeyMap	ServerElement::_initKeyMap(void) {
	KeyMap	ret;

	ret["autoindex"] = KEY::AUTOINDEX;
	ret["client_max_body_size"] = KEY::CLIENT_MAX_BODY_SIZE;
	ret["error_page"] = KEY::ERROR_PAGE;
	ret["index"] = KEY::INDEX;
	ret["listen"] = KEY::LISTEN;
	ret["location"] = KEY::LOCATION;
	ret["location_trie"] = KEY::LOCATION_TRIE;
	ret["return"] = KEY::RETURN;
	ret["root"] = KEY::ROOT;
	ret["server_name"] = KEY::SERVER_NAME;
	return (ret);
}

ServerElement::ServerElement(void): _element_map(ElementMap()) {}
ServerElement::ServerElement(std::ifstream &infile) throw(std::exception): _element_map(ElementMap()) {
	try {
		this->_parse(infile);
	} catch (const std::exception &e) {
		Logger::getInstance().error(e.what());
		throw (FailToParseException());
	}
}
ServerElement::~ServerElement(void) {}
ServerElement::ServerElement(const ServerElement &ref): _element_map(ref._element_map) {}
ServerElement	&ServerElement::operator=(const ServerElement &rhs) {
	if (this != &rhs) {
		this->~ServerElement();
		new(this) ServerElement(rhs);
	}
	return (*this);
}

ServerElement::iterator			ServerElement::begin(void) { return (this->_element_map.begin()); }
ServerElement::iterator			ServerElement::end(void) { return (this->_element_map.end()); }
ServerElement::iterator			ServerElement::find(KEY::e_key key) {
	ElementMap::iterator	it = this->_element_map.find(key);

	if (it == this->_element_map.end()) {
		return (this->_element_map.end());
	}
	return (it);
}
ServerElement::const_iterator	ServerElement::begin(void) const { return (this->_element_map.begin()); }
ServerElement::const_iterator	ServerElement::end(void) const { return (this->_element_map.end()); }
ServerElement::const_iterator	ServerElement::find(KEY::e_key key) const {
	ElementMap::const_iterator	it = this->_element_map.find(key);

	if (it == this->_element_map.end()) {
		return (this->_element_map.end());
	}
	return (it);
}

bool	ServerElement::_parse(std::ifstream &infile) throw(std::exception) {
	std::string				token;
	KeyMap::const_iterator	key_it;
	
	if ((infile >> token) && (token != "{")) {
		throw (InvalidSyntaxException());
	}
	while (infile >> token) {
		if (token == "}") {
			break;
		}
		if ((key_it = this->_key_map.find(token)) == this->_key_map.end()) {
			throw (InvalidElementException());
		}
		if (key_it->second != KEY::LOCATION && this->_element_map.find(key_it->second) != this->_element_map.end()) {
			throw (DuplicatedElementException());
		}
		try {
			if (key_it->second == KEY::LOCATION) {
				if (this->_element_map.find(KEY::LOCATION_TRIE) == this->_element_map.end()) {
					this->_element_map[KEY::LOCATION_TRIE] = ft::static_pointer_cast<ConfigElement>(ConfigElementFactory::getInstance().create("location_trie", infile));
				} else {
					ft::static_pointer_cast<LocationTrieElement>(this->_element_map[KEY::LOCATION_TRIE])->insert(infile);
				}
			} else {
				this->_element_map[key_it->second] = ft::static_pointer_cast<ConfigElement>(ConfigElementFactory::getInstance().create(key_it->first, infile));
			}
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToCreateElementException());
		}
	}
	return (true);
}

ServerElement::ElementPtr		&ServerElement::operator[](KEY::e_key key) { return (this->_element_map[key]); }
const ServerElement::ElementPtr	&ServerElement::operator[](KEY::e_key key) const { return (this->_element_map.at(key)); }

const char	*ServerElement::FailToParseException::what(void) const throw() { return ("ServerElement: Fail to Parse"); }
const char	*ServerElement::InvalidSyntaxException::what(void) const throw() { return ("ServerElement: Invalid Syntax"); }
const char	*ServerElement::InvalidElementException::what(void) const throw() { return ("ServerElement: Invalid Element"); }
const char	*ServerElement::DuplicatedElementException::what(void) const throw() { return ("ServerElement: Duplicated Element"); }
const char	*ServerElement::FailToCreateElementException::what(void) const throw() { return ("ServerElement: Fail to Create Element"); }

std::ostream	&operator<<(std::ostream &os, const ServerElement &rhs) {
	os << "\t\t\t\t\t\t\t\t" << "root: " << *(ft::static_pointer_cast<RootElement>(rhs.find(ServerElement::KEY::ROOT)->second)) << '\n';
	os << "\t\t\t\t\t\t\t\t" << "index: " << *(ft::static_pointer_cast<IndexElement>(rhs.find(ServerElement::KEY::INDEX)->second)) << '\n';
	os << "\t\t\t\t\t\t\t\t" << "error_page: " << *(ft::static_pointer_cast<ErrorPageElement>(rhs.find(ServerElement::KEY::ERROR_PAGE)->second)) << '\n';
	os << "\t\t\t\t\t\t\t\t" << "client_max_body_size: " << *(ft::static_pointer_cast<ClientMaxBodySizeElement>(rhs.find(ServerElement::KEY::CLIENT_MAX_BODY_SIZE)->second)) << '\n';
	return (os);
}