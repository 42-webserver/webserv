#include "../../../incs/http/parser/HttpRequestParser.hpp"

std::string createMultipartFormData(const std::string& boundary, const std::string& fieldName, const std::string& fieldValue, const std::string& fileName, const std::string& fileContent) {
	std::string formData;
	formData += "--" + boundary + "\r\n";
	formData += "Content-Disposition: form-data; name=\"" + fieldName + "\"\r\n";
	formData += "\r\n";
	formData += fieldValue + "\r\n";

	formData += "--" + boundary + "\r\n";
	formData += "Content-Disposition: form-data; name=\"" + fieldName + "\"; filename=\"" + fileName + "\"\r\n";
	formData += "Content-Type: application/octet-stream\r\n";
	formData += "\r\n";
	formData += fileContent + "\r\n";

	formData += "--" + boundary + "--\r\n";

	return formData;
}

std::string createMultipartRequestBody(const std::string& fieldName, const std::string& fieldValue, const std::string& fileName, const std::string& fileContent) {
	std::string requestBody;
	std::string boundary = "----Boundary" + std::to_string(std::rand());
	requestBody += "POST / HTTP/1.1\r\n";
	requestBody += "Host: localhost:8080\r\n";
	requestBody += "User-Agent: curl/7.64.1\r\n";
	requestBody += "Content-Type: multipart/form-data; boundary=" + boundary + "\r\n";
	requestBody += "Content-Length: " + std::to_string(createMultipartFormData(boundary, fieldName, fieldValue, fileName, fileContent).length()) + "\r\n";
	requestBody += "\r\n";
	requestBody += createMultipartFormData(boundary, fieldName, fieldValue, fileName, fileContent);

	return requestBody;
}

int main(){
	std::string fieldName = "text_field";
	std::string fieldValue = "This is a text field value.";
	std::string fileName = "example.txt";
	std::string fileContent = "This is the content of the file.";
	
	 std::string multipartBodyRequest = createMultipartRequestBody(fieldName, fieldValue, fileName, fileContent);
	
	std::vector<char> httpReqeustBuffer;
	httpReqeustBuffer.assign(multipartBodyRequest.begin(), multipartBodyRequest.end());
	HttpRequestParser httpRequestParser;
	httpRequestParser.parseRequest(httpReqeustBuffer, 1000000);
	ft::shared_ptr <HttpRequest> httpRequest = httpRequestParser.getHttpRequest();
	std::cout << httpRequest->getUri() << "\n" << \
	httpRequest->getProtocol() << std::endl;
	std::cout << httpRequest->getMethod() << std::endl;
	std::cout << httpRequest->getVersion() << std::endl;
	std::cout << httpRequestParser.getState() << std::endl;
	std::cout << httpRequest->isBodyLong() << std::endl;
	std::vector<MultipartRequest> multiparts = httpRequest->getMultipartRequests();

	std::cout << "Multipart requests: " << multiparts.size() << std::endl;
	for(size_t i = 0; i < multiparts.size(); i++){
		std::cout << "part " << i << std::endl;
		if (httpRequest->isBodyLong())
			std::cout << multiparts[i].getBodyDataFilename() << std::endl;
		else
			std::cout << multiparts[i].getMemoryBody().data() << std::endl;
		std::multimap<std::string, std::string> _headers = multiparts[i].getHeaders();
		std::multimap<std::string, std::string>::iterator it;
		for (it = _headers.begin(); it != _headers.end(); it++)
		{
			std::cout << it->first << " : " << it->second << std::endl;
		}
	}

}