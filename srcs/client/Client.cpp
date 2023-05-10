#include "../../incs/client/Client.hpp"

Client::Client(void): Data() {}
Client::Client(int fd): Data(fd) {}
Client::~Client(void) {}