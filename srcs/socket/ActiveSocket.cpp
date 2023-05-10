#include "../../incs/socket/ActiveSocket.hpp"

ActiveSocket::ActiveSocket(void): Socket() {}
ActiveSocket::ActiveSocket(int fd): Socket(fd) {}
ActiveSocket::~ActiveSocket(void) {}