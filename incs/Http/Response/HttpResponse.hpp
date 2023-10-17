#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include "../Utils/HttpStatus.hpp"
#include "../Utils/Cookie.hpp"
#include <iostream>
#include <Buffer/Buffer/IoReadAndWriteBuffer.hpp>
#include <../../../libs/Library/Optional.hpp>

typedef enum{
	NotSet,
	normalSize,
	BigSize
}	e_ResponseSize;

typedef enum{
	NotSetting,
	Reading,
	ReadingDone
}	e_File_Sync;
class FileManager;
class HttpResponse
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
	std::string 		_version;
	std::vector<char>	_body;
	std::string 		_protocol;
	HttpStatusCode 		_statusCode;
	std::string 		_fileName;
	std::multimap<std::string, std::string> _headers;
	std::vector<Cookie> _cookies;

//Relation with file Interface
private:
	e_ResponseSize		_responseSize;
	ft::Optional<e_File_Sync>	_fileSync;
	ft::shared_ptr<IoReadAndWriteBuffer> _BigSizeBuffer;
	std::vector<char>	_NormalCaseBuffer;
public:
	void setFileSync(e_File_Sync fileSync, AccessKey key);
	void setResponseSize(e_ResponseSize responseSize, AccessKey key);
	ft::Optional<e_File_Sync> getFileSync(AccessKey key);
	std::vector<char>& getNormalCaseBuffer(AccessKey key);
	ft::shared_ptr<IoReadAndWriteBuffer> &getBigSizeBuffer(AccessKey key);
	void allocateBigSizeBuffer(AccessKey key);
//file Interface end

public:
	HttpResponse();
	~HttpResponse();
	void setFileName(std::string & fileName);
	void addHeader(const std::string & key, const std::string & value);
	void setStatusCode(HttpStatusCode code);
	void addCookie(const std::string & key, const std::string & value);
	std::string getVersion();
	std::string getFileName();
	std::vector<char> &getBody();
	std::string getProtocol();
	std::string getReasonPhrase();
	int getStatusCode();
	const std::multimap<std::string, std::string> &getHeaders() const;
	std::string getHeader(const std::string & key);
	const std::vector<Cookie> &getCookies() const;

	friend	std::ostream &operator<<(std::ostream & os, const HttpResponse & response);

};

#endif
