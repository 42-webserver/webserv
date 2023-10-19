#ifndef ERRPR_PAGE_HANDLER_HPP
#define ERRPR_PAGE_HANDLER_HPP

#include "../Utils/HttpStatus.hpp"
#include "../Response/HttpResponse.hpp"
#include "../../../libs/shared_ptr/shared_ptr.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Client/Client.hpp>
class ErrorPageHandler
{
public:
	static void getErrorPageResponseTo(ft::shared_ptr<Client> client, HttpStatusCode status);
private:
	static std::vector<char> getErrorPage(HttpStatusCode status);

};

#endif