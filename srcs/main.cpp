#include "../incs/config/Config.hpp"
#include "../incs/server/Server.hpp"
#include "../incs/err/err.hpp"
#include <iostream>

int	main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "\033[31m" << "Usage: " << argv[0] << " <config_file>" << "\033[0m" << '\n';
		return (INVALID_ARG);
	}

	Config				config(argv[1]);
	std::vector<Server>	servers;

	try {
		config.setServers(servers);
	} catch (std::exception &e) {
		std::cerr << "\033[31m" << "Error: " << e.what() << "\033[0m" << '\n';
		return (GENERIC_ERR);
	}
	return (0);
}