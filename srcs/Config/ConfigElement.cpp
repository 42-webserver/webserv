#include "../../incs/Config/ConfigElement.hpp"
#include "../../incs/Config/AliasElement.hpp"
#include "../../incs/Config/AutoIndexElement.hpp"
#include "../../incs/Config/ClientMaxBodySizeElement.hpp"
#include "../../incs/Config/ErrorPageElement.hpp"
#include "../../incs/Config/IndexElement.hpp"
#include "../../incs/Config/ListenElement.hpp"
#include "../../incs/Config/LocationElement.hpp"
#include "../../incs/Config/LocationTrieElement.hpp"
#include "../../incs/Config/ReturnElement.hpp"
#include "../../incs/Config/RootElement.hpp"
#include "../../incs/Config/ServerElement.hpp"
#include "../../incs/Config/ServerNameElement.hpp"

ConfigElement::ConfigElement(void) {}
ConfigElement::ConfigElement(std::ifstream &infile) throw(std::exception) { (void) infile; }
ConfigElement::~ConfigElement(void) {}
ConfigElement::ConfigElement(const ConfigElement &ref) { (void) ref; }
ConfigElement	&ConfigElement::operator=(const ConfigElement &rhs) { (void) rhs; return (*this); }

ConfigElementFactory::ConfigElementFactory(void) {}
ConfigElementFactory::~ConfigElementFactory(void) {}
ConfigElementFactory::ConfigElementFactory(const ConfigElementFactory &ref) { (void) ref; }
ConfigElementFactory	&ConfigElementFactory::operator=(const ConfigElementFactory &rhs) { (void) rhs; return (*this); }

ConfigElementFactory	&ConfigElementFactory::getInstance(void) {
	static ConfigElementFactory	instance;

	return (instance);
}

ConfigElementFactory::ElementPtr	ConfigElementFactory::create(const std::string &element, std::ifstream &infile) const throw(std::exception) {
	ElementPtr	ret;

	if (element == "alias") {
		ret = ft::make_shared<AliasElement>(infile);
	} else if (element == "autoindex") {
		ret = ft::make_shared<AutoIndexElement>(infile);
	} else if (element == "client_max_body_size") {
		ret = ft::make_shared<ClientMaxBodySizeElement>(infile);
	} else if (element == "error_page") {
		ret = ft::make_shared<ErrorPageElement>(infile);
	} else if (element == "index") {
		ret = ft::make_shared<IndexElement>(infile);
	} else if (element == "listen") {
		ret = ft::make_shared<ListenElement>(infile);
	} else if (element == "location") {
		ret = ft::make_shared<LocationElement>(infile);
	} else if (element == "location_trie") {
		ret = ft::make_shared<LocationTrieElement>(infile);
	} else if (element == "return") {
		ret = ft::make_shared<ReturnElement>(infile);
	} else if (element == "root") {
		ret = ft::make_shared<RootElement>(infile);
	} else if (element == "server") {
		ret = ft::make_shared<ServerElement>(infile);
	} else if (element == "server_name") {
		ret = ft::make_shared<ServerNameElement>(infile);
	} else {
		throw (InvalidElementException());
	}
	return (ret);
}

const char	*ConfigElementFactory::InvalidElementException::what(void) const throw() { return ("ConfigElementFactory: Invalid element"); }