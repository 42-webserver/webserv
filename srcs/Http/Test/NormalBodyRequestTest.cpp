#include "../../../incs/Http/Parser/HttpRequestParser.hpp"

int main(){
	std::string bodyData = "This is the request body data.";
	std::string normalBodyRequest = "GET / Http/1.1\r\n"
						  "Host: localhost:8080\r\n"
						  "User-Agent: curl/7.64.1\r\n"
						  "Accept: */*\r\n"
						  "Content-Length: " + std::to_string(bodyData.length()) + "\r\n"
						  "\r\n" + bodyData;
	
	
	std::vector<char> httpReqeustBuffer;
	httpReqeustBuffer.assign(normalBodyRequest.begin(), normalBodyRequest.end());
	HttpRequestParser httpRequestParser;
	httpRequestParser.parseRequest(httpReqeustBuffer, 1000000);
	ft::shared_ptr <HttpRequest> httpRequest = httpRequestParser.getHttpRequest();
	std::cout << httpRequest->getUri() << "\n" << \
	httpRequest->getProtocol() << std::endl;
	std::cout << httpRequest->getMethod() << std::endl;
	std::cout << httpRequest->getVersion() << std::endl;
	std::cout << httpRequestParser.getState() << std::endl;
	std::cout << httpRequest->getBody().data() << std::endl;
	std::cout << httpRequest->isBodyLong() << std::endl;
	std::cout << httpRequest->getBodyDataFilename() << std::endl;

	std::cout << "== 나뉘어져 들어오는 request ==" << std::endl;

	std::string normalBodyRequest1 = "GET / Http/1.1\r\n"
						  "Host: localhost:8080\r\n"
						  "User-Agent: curl/7.64.1\r\n";
	
	std::string normalBodyRequest2 = "Accept: */*\r\n"
						  "Content-Length: " + std::to_string(bodyData.length()) + "\r\n"
						  "\r\n" + bodyData;
	
	std::vector<char> httpReqeustBuffer1;
	httpReqeustBuffer1.assign(normalBodyRequest1.begin(), normalBodyRequest1.end());
	std::vector<char> httpReqeustBuffer2;
	httpReqeustBuffer2.assign(normalBodyRequest2.begin(), normalBodyRequest2.end());

	HttpRequestParser httpRequestParser2;
	httpRequestParser2.parseRequest(httpReqeustBuffer1, 1000000);
	httpRequestParser2.parseRequest(httpReqeustBuffer2, 1000000);
	ft::shared_ptr <HttpRequest> httpRequest2 = httpRequestParser2.getHttpRequest();

	std::cout << httpRequest2->getUri() << "\n" << \
	httpRequest2->getProtocol() << std::endl;
	std::cout << httpRequest2->getMethod() << std::endl;
	std::cout << httpRequest2->getVersion() << std::endl;
	std::cout << httpRequestParser2.getState() << std::endl;
	std::cout << httpRequest2->getBody().data() << std::endl;
	std::cout << httpRequest2->isBodyLong() << std::endl;
	std::cout << httpRequest2->getBodyDataFilename() << std::endl;

}