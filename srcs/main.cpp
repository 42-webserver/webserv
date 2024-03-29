#include "../incs/Config/Config.hpp"
#include "../incs/Server/Webserv.hpp"
#include "../incs/Log/Logger.hpp"
#include "../incs/err/err.hpp"
#include <iostream>


int	main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "\033[31m" << "Usage: " << argv[0] << " <config_file>" << "\033[0m" << '\n';
		return (INVALID_ARG);
	}

	try {
		Config	config(argv[1]);
		Webserv	webserv;

		webserv.run(config);
	} catch (const std::exception &e) {
		std::cerr << "\033[31m" << e.what() << "\033[0m" << '\n';
		Logger::getInstance().error(e.what());
		return (GENERIC_ERR);
	}
	return (0);
}