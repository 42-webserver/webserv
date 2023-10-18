#include <Http/Handler/ErrorPageHandler.hpp>



// ft::shared_ptr<HttpResponse> ErrorPageHandler::getErrorPageResponse(HttpStatusCode status) {
// 	ft::shared_ptr<HttpResponse> response = ft::make_shared<HttpResponse>();
// 	std::vector<char> errorPage = getErrorPage(status);
// 	response->setBody(errorPage);
// 	response->addHeader("Content-Type", "text/html");
// 	response->setStatusCode(status);
// 	return response;
// }

// std::vector<char> ErrorPageHandler::getErrorPage(HttpStatusCode status) {
// 	std::vector<char> errorPage;

// 	std::stringstream ss;
// 	ss << "<html><head><title>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</title></head><body><h1>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</h1></body></html>";
// 	errorPage.insert(errorPage.end(), ss.str().begin(), ss.str().end());
	
// 	return errorPage;
// }
