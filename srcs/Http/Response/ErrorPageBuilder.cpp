#include <Http/Response/ErrorPageBuilder.hpp>
#include <Http/Response/HttpResponse.hpp>
#include <Client/Client.hpp>

ErrorPageBuilder::ErrorPageBuilder(ft::shared_ptr<Client> client, HttpStatusCode status) :
HttpResponseBuilder(client), _status(status) {}

ErrorPageBuilder::~ErrorPageBuilder(void) {}


std::string ErrorPageBuilder::getErrorPage(HttpStatusCode status) {

    std::stringstream ss;
    ss << HttpStatus::getStatusCode(status);
    // Get status code and reason phrase
    std::string statusCode = ss.str();
    std::string reasonPhrase = HttpStatus::getReasonPhrase(status);

    std::string errorPage;
    // Build the error page
    errorPage += "<!DOCTYPE html>";
    errorPage += "<html lang=\"en\">";
    errorPage += "<head>";
    errorPage += "<meta charset=\"UTF-8\">";
    errorPage += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    errorPage += "<title>" + statusCode + " " + reasonPhrase + "</title>";
    errorPage += "<style>";
    errorPage += "body { font-family: Arial, sans-serif; text-align: center; padding-top: 20%; }";
    errorPage += "h1 { font-size: 48px; }";
    errorPage += "</style>";
    errorPage += "</head>";
    errorPage += "<body>";
    errorPage += "<h1>" + statusCode + " " + reasonPhrase + "</h1>";
    errorPage += "</body>";
    errorPage += "</html>";

    std::cerr << "ErrorPageBuilder::getErrorPage: size = " << errorPage.size() << std::endl;
	return errorPage;
}

//Todo :check
void ErrorPageBuilder::buildResponseHeader(std::vector<char> &buffer) {
    ft::shared_ptr<HttpResponse> response = this->getClient()->getResponse();
   std::string errorPage = getErrorPage(this->_status);

    std::cerr << "buffer size: " << buffer.size() << std::endl;
	response->setResponseSize(NormalSize, HttpResponse::AccessKey());
    std::string ContenTypeHeader = "Content-Type : text/html";
    _setStatusCode(this->_status);
    std::cerr << "ErrorPageBuilder::buildResponseHeader:  errorPage.size() = " << errorPage.size() << std::endl;
    _allocContentLength(ContentLength::e_content_length_header, errorPage.size());
    _buildDefaultResponseHeader(buffer);

    std::string header = ContenTypeHeader + "\r\n\r\n";
    buffer.insert(buffer.end(), header.begin(), header.end());

    //body/
    buffer.insert(buffer.end(), errorPage.begin(), errorPage.end());    

    this->getClient()->setResponse(response);
}