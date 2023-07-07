#include "../../incs/Config/ConfigElement.hpp"
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

ConfigElement	*ConfigElementFactory::create(const std::string &element, std::ifstream &infile) const throw(std::exception) {
	ConfigElement	*ret;

	if (element == "autoindex") {
		ret = new AutoIndexElement(infile);
	} else if (element == "client_max_body_size") {
		ret = new ClientMaxBodySizeElement(infile);
	} else if (element == "error_page") {
		ret = new ErrorPageElement(infile);
	} else if (element == "index") {
		ret = new IndexElement(infile);
	} else if (element == "listen") {
		ret = new ListenElement(infile);
	} else if (element == "location") {
		ret = new LocationElement(infile);
	} else if (element == "location_trie") {
		ret = new LocationTrieElement(infile);
	} else if (element == "return") {
		ret = new ReturnElement(infile);
	} else if (element == "root") {
		ret = new RootElement(infile);
	} else if (element == "server") {
		ret = new ServerElement(infile);
	} else if (element == "server_name") {
		ret = new ServerNameElement(infile);
	} else {
		throw (InvalidElementException());
	}
	return (ret);
}

const char	*ConfigElementFactory::InvalidElementException::what(void) const throw() { return ("ConfigElementFactory: Invalid element"); }