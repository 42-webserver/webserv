#include "../../../incs/http/handler/ErrorPageHandler.hpp"

std::string * ErrorPageHandler::getErrorPage(HttpStatusCode status)
{
	std::string *errorPage = new std::string();
	std::string errorPageName = getErrorPageName(status);
	std::ifstream ifs;

	ifs.open(errorPageName.c_str());
	 if (ifs.is_open()) {
		while (!ifs.eof()) {
			std::string str;
			std::getline(ifs, str);
			errorPage->append(str);
		}
		ifs.close();
	}
	else{
		std::stringstream ss;
		ss << "<html><head><title>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</title></head><body><h1>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</h1></body></html>";
		*errorPage = ss.str();
	}
	return errorPage;
}

std::string ErrorPageHandler::getErrorPageName(HttpStatusCode status)
{
	std::string errorPageName;
	std::stringstream ss;
	std::string errorPagePath = "html/error_pages/";

	ss << HttpStatus::getStatusCode(status);
	errorPageName = errorPagePath + ss.str() + ".html";
	return errorPageName;
}