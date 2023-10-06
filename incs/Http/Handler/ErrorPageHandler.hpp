#ifndef ERRPR_PAGE_HANDLER_HPP
#define ERRPR_PAGE_HANDLER_HPP

#include "../Utils/HttpStatus.hpp"
#include "../Response/HttpResponse.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class ErrorPageHandler
{
public:
	static ft::shared_ptr<HttpResponse> getErrorPageResponse(HttpStatusCode status);
private:
	static std::vector<char> getErrorPage(HttpStatusCode status);

};

#endif