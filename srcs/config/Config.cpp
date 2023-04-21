#include "../../incs/config/Config.hpp"

Config::Config(const char *file_name): _file_name(file_name) {}
Config::~Config(void) {}

// Util
//bool	Config::setServer(Server &server, File_pointer fp) const {
//	return (true);
//}

//bool	Config::setServers(std::vector<Server> &servers) throw(std::exception) {
	//File_pointer fp;

	//while (fp != eof()) {
	//	Server	server;

	//	try {
	//		setServer(server, fp);
	//	} catch (Config::ParsingException &e) {
	//		throw (e);
	//	} catch (...) {
	//		throw (Config::UnknownException());
	//	}
	//	servers.push_back(server);
	//}
	//return (true);
//}

// Exception
const char*	Config::ParsingException::what() const throw() { return ("ParsingException"); }

const char*	Config::UnknownException::what() const throw() { return ("UnknownException"); }