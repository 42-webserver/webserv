#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include "../Exception/BadRequestException.hpp"
#include "MultipartRequest.hpp"
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
#include "../../../libs/shared_ptr/shared_ptr.hpp"

typedef enum BodyType{
		NORMAL,
		CHUNKED,
		MULTIPART_FORM_DATA
} BodyType;

typedef enum {
	NoneSetting,
	Writing,
	WritingDone
}	e_file_upload_sync;

static const int noContentLength = -1;

class HttpRequest
{
public:
	class AccessKey
	{
		friend class FileManager;
		private:
			AccessKey();
			~AccessKey();
	};



private:
	bool									_error;
	HttpStatusCode							_errorStatusCode;
	std::string		 						_method;
	std::string		 						_uri;
	std::string		 						_version;
	ft::shared_ptr<IoReadAndWriteBuffer>    _body;
	std::string		 						_protocol;
	BodyType								_bodyType;


	std::multimap<std::string, std::string> _headers;
	std::map<std::string, std::string>	  	_queries;
	std::map<std::string, std::string>	  	_cookies;
	std::vector<MultipartRequest>		 	_multipartRequests;
	//debug
public:
	void _printBody();

//file Interface
private:
	e_file_upload_sync						_fileUploadSync;
//file Interface
public:
	e_file_upload_sync getFileUploadSync(AccessKey key);
	void setFileUploadSync(e_file_upload_sync fileUploadSync, AccessKey key);


public:
	HttpRequest();
	~HttpRequest();
	void addHeader(const std::string & header);
	void setStartLine(std::string line);
	ssize_t insertBody(std::vector<char> &buffer);
	ssize_t insertBody(void);
	std::string getMethod();
	std::string getUri();
	void setUri(std::string uri);
	std::string getVersion();
	std::string getProtocol();
	std::string getHost();
	ft::shared_ptr<IoReadAndWriteBuffer> getBody();
	size_t getBodySize() const;
	std::vector<MultipartRequest>	&getMultipartRequests();
	std::multimap<std::string, std::string> &getHeaders();
	std::map<std::string, std::string> getQueries();
	std::map<std::string, std::string> getCookies();
	ssize_t  getContentLength();
	BodyType getBodyType();
	void setBodyType(BodyType bodyType);
	std::string getHeader(const std::string & key);
	bool isPending();
	void setFinished();
	bool isError();
	void setError(HttpStatusCode errorCode);
	HttpStatusCode getErrorStatusCode();
	friend std::ostream &operator<<(std::ostream & os, const HttpRequest & request);

private:
	void setQuery(std::string & uri);
	void setCookie(std::string & cookie);
	bool esacpeComma(std::string & value);
	void handleMultipleValueHeader(std::string & value, std::string & key);
};

#endif