#include <Http/Handler/ErrorPageHandler.hpp>



// void ErrorPageHandler::getErrorPageResponseTo(ft::shared_ptr<Client> client, HttpStatusCode status) {
// 	ft::shared_ptr<HttpResponse> response = ft::make_shared<HttpResponse>();
// 	std::vector<char> errorPage = getErrorPage(status);
// 	response->setResponseSize(NormalSize, HttpResponse::AccessKey());
//     response->getNormalCaseBuffer(HttpResponse::AccessKey()).
//     insert(response->getNormalCaseBuffer(HttpResponse::AccessKey()).end(), errorPage.begin(), errorPage.end());
// 	response->addHeader("Content-Type", "text/html");
// 	response->setStatusCode(status);
// 	response->setCanSending();
// 	client->setResponse(response);
// }

// std::vector<char> ErrorPageHandler::getErrorPage(HttpStatusCode status) {
// 	std::vector<char> errorPage;

// 	std::stringstream ss;
// 	ss << "<html><head><title>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</title></head><body><h1>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</h1></body></html>";
// 	errorPage.insert(errorPage.end(), ss.str().begin(), ss.str().end());
	
// 	return errorPage;
// }
