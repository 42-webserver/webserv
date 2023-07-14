#include "../../../incs/Http/Handler/ErrorPageHandler.hpp"

ft::shared_ptr<HttpResponse> ErrorPageHandler::getErrorPageResponse(HttpStatusCode status) {
	ft::shared_ptr<HttpResponse> response = ft::make_shared<HttpResponse>();
	std::vector<char> errorPage = getErrorPage(status);
	response->setBody(errorPage);
	response->addHeader("Content-Type", "text/html");
	response->setStatusCode(status);
	return response;
}

std::vector<char> ErrorPageHandler::getErrorPage(HttpStatusCode status) {
	std::vector<char> errorPage;
	std::string errorPageName = getErrorPageName(status);
	std::ifstream ifs;

	ifs.open(errorPageName.c_str());
	 if (ifs.is_open()) {
		while (!ifs.eof()) {
			std::string str;
			std::getline(ifs, str);
			errorPage.insert(errorPage.end(), str.begin(), str.end());
		}
		ifs.close();
	}
	else{
		std::stringstream ss;
		ss << "<html><head><title>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</title></head><body><h1>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</h1></body></html>";
		errorPage.insert(errorPage.end(), ss.str().begin(), ss.str().end());
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