#include "../../../incs/VirtualServerMap/VirtualServerMap.hpp"
//  g++ VirtualServerMap.cpp ./../Config/Config.cpp ./../Server/Server.cpp ./../Socket/Socket.cpp ./../Server/Location.cpp ./Test/thisTest.cpp
    // Create a test server

int main() {
    VirtualServerMap vServerMap;

    // Create a config map
    Config::map configMap;
    configMap[Config::KEYS[0]] = std::vector<std::string>(1, "8080"); // Port
    configMap[Config::KEYS[1]] = std::vector<std::string>(1, "localhost"); // Names
    configMap[Config::KEYS[2]] = std::vector<std::string>(1, "/var/www/html"); // Root
    configMap[Config::KEYS[3]] = std::vector<std::string>(1, "index.html"); // Indexes
    configMap[Config::KEYS[4]] = std::vector<std::string>(1, "404.html"); // Default error page
    configMap[Config::KEYS[5]] = std::vector<std::string>(1, "1000000"); // Client max body size

    // Create a Config object
    Config config(configMap);

    // Create a Server object using the Config object
    Server testServer(config);

    virtualServerMap::KeyType key(8080, "localhost");
    std::vector<std::string> serverNames = { "test-server" };
    vServerMap.addOrUpdateServer(key, serverNames, &testServer);

    // Get the value (map) for the given key from the virtualServerMap
    std::map<std::vector<std::string>, Server*>& serverMap = vServerMap.getVertualServerMap(key);

    // Print the server details
    for (const auto& server : serverMap) {
        std::cout << "Server Names: ";
        for (const auto& name : server.first) {
            std::cout << name << " ";
        }
        std::cout << "\n";
        std::cout << "Server Port: " << testServer.getPort() << "\n";
        std::cout << "Server Root: " << testServer.getRoot() << "\n";
        // Print other server details as needed
    }

    return 0;
}