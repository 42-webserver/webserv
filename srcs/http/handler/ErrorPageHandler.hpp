#ifndef ERRPR_PAGE_HANDLER_HPP
#define ERRPR_PAGE_HANDLER_HPP

#include "../utils/HttpStatus.hpp"
#include <fstream>
#include <sstream>
#include <string>

class ErrorPageHandler
{
public:
    static std::string * getErrorPage(HttpStatusCode status);
private:
    static std::string getErrorPageName(HttpStatusCode status);

};

#endif