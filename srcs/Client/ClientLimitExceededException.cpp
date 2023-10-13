#include <Client/ClientLimitExceededException.hpp>

const char *ClientLimitExceededException::what() const throw() {
    return "Client limit exceeded";
}