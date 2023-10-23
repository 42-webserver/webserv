#include <Http/Response/ErrorPageBuilder.hpp>
#include <Http/Response/HttpResponse.hpp>
#include <Client/Client.hpp>

ErrorPageBuilder::ErrorPageBuilder(ft::shared_ptr<Client> client, HttpStatusCode status) :
HttpResponseBuilder(client), _status(status) {}

ErrorPageBuilder::~ErrorPageBuilder(void) {}


std::vector<char> ErrorPageBuilder::getErrorPage(HttpStatusCode status) {
	std::vector<char> errorPage;

	std::stringstream ss;
	ss << "<html><head><title>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</title></head><body><h1>" << HttpStatus::getStatusCode(status) << " " << HttpStatus::getReasonPhrase(status) << "</h1></body></html>";
	errorPage.insert(errorPage.end(), ss.str().begin(), ss.str().end());
	
	return errorPage;
}

//Todo :check
void ErrorPageBuilder::buildResponseHeader(std::vector<char> &buffer) {
    ft::shared_ptr<HttpResponse> response = this->getClient()->getResponse();
    std::vector<char> errorPage = getErrorPage(this->_status);
	response->setResponseSize(NormalSize, HttpResponse::AccessKey());
    std::string ContenTypeHeader = "Content-Type : text/html";
    _setStatusCode(this->_status);
    _allocContentLength(ContentLength::e_content_length_header, errorPage.size());
    _buildDefaultResponseHeader(buffer);

    std::string header = ContenTypeHeader + "\r\n\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());

    //body/
    buffer.insert(buffer.end(), errorPage.begin(), errorPage.end());    

    this->getClient()->setResponse(response);
}