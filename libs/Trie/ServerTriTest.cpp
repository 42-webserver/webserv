#include "./ServerTri.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "\033[31m" << "Usage: " << argv[0] << " <config_file>" << "\033[0m" << '\n';
		return (EXIT_FAILURE);
	}
	Config config;
	config = Config(argv[1]);
	config.startParse();

    const std::vector<Config::map> server_configs = config.getConfigMaps();
	std::vector<ft::shared_ptr<VirtualServer> > virtualServers;
	ServerTrie serverTrie;
	for (std::vector<Config::map>::const_iterator curServerConfig = server_configs.begin(); 
	curServerConfig != server_configs.end(); ++curServerConfig) {
		ft::shared_ptr<VirtualServer> current(new VirtualServer(*curServerConfig));
		virtualServers.push_back(current);
		std::cout << virtualServers.back().get()->getIP() << std::endl;
		serverTrie.insert(current);
	}
	serverTrie.checkingSocketInTrie();
	std::cout << serverTrie << std::endl;
	return (0);
}