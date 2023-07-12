#include "../../../incs/Http/Exception/NotFoundException.hpp"

const char* NotFoundException::what() const throw(){
    return "404 Not Found";
}