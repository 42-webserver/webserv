#include "../../incs/Config/ServerElement.hpp"
#include "../../incs/Config/LocationTrieElement.hpp"
#include "../../incs/Log/Logger.hpp"

const ServerElement::KeyMap	ServerElement::_key_map = ServerElement::_initKeyMap();
ServerElement::KeyMap	ServerElement::_initKeyMap(void) {
	KeyMap	ret;

	ret["auto_index"] = KEY::AUTO_INDEX;
	ret["client_max_body_size"] = KEY::CLIENT_MAX_BODY_SIZE;
	ret["error_page"] = KEY::ERROR_PAGE;
	ret["index"] = KEY::INDEX;
	ret["listen"] = KEY::LISTEN;
	ret["location"] = KEY::LOCATION;
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
ServerElement::~ServerElement(void) {
	for (ElementMap::iterator it = this->_element_map.begin(); it != this->_element_map.end(); ++it) {
		if (it->second != NULL) {
			delete (it->second);
		}
	}
}
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
				if (this->_element_map[key_it->second] == NULL) {
					this->_element_map[key_it->second] = ConfigElementFactory::getInstance().create("location_trie", infile);
				} else {
					((LocationTrieElement *) this->_element_map[key_it->second])->insert(infile);
				}
			} else {
				this->_element_map[key_it->second] = ConfigElementFactory::getInstance().create(key_it->first, infile);
			}
		} catch (const std::exception &e) {
			Logger::getInstance().error(e.what());
			throw (FailToCreateElementException());
		}
	}
	return (true);
}

ConfigElement	*&ServerElement::operator[](KEY::e_key key) { return (this->_element_map[key]); }
const ConfigElement	*&ServerElement::operator[](KEY::e_key key) const { return ((const ConfigElement *&) this->_element_map.at(key)); }

const char	*ServerElement::FailToParseException::what(void) const throw() { return ("ServerElement: Fail to Parse"); }
const char	*ServerElement::InvalidSyntaxException::what(void) const throw() { return ("ServerElement: Invalid Syntax"); }
const char	*ServerElement::InvalidElementException::what(void) const throw() { return ("ServerElement: Invalid Element"); }
const char	*ServerElement::DuplicatedElementException::what(void) const throw() { return ("ServerElement: Duplicated Element"); }
const char	*ServerElement::FailToCreateElementException::what(void) const throw() { return ("ServerElement: Fail to Create Element"); }