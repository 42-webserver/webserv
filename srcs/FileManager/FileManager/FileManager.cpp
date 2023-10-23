#include <FileManager/FileManager/FileManager.hpp>

FileManager *FileManager::_instance = NULL;
FileManager::FileManager(void) {}
FileManager::~FileManager(void) {}

FileManager    &FileManager::getInstance(void) {
    if (FileManager::_instance == NULL) {
        FileManager::_instance = new FileManager();
    }
    return (*FileManager::_instance);
}
void FileManager::_readFile(const std::string &uri, ft::shared_ptr<HttpResponse> response) {
    //Todo :: 
    FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
    FileData &fileData = fileTableManager.getFileData(uri);

    EventFactory &eventFactory = EventFactory::getInstance();
    ft::shared_ptr<SyncroFileDataAndReader> syncroFileDataAndReader = fileData.buildSyncroFileDataAndReader();
    response->allocateBigSizeBuffer(HttpResponse::AccessKey());
    EventDto eventDto(response->getBigSizeBuffer(HttpResponse::AccessKey()), uri, "r");
    ReadEventFromFile* readEventFromFile = static_cast<ReadEventFromFile*>(eventFactory.createEvent(ft::FILE_READ_EVENT, eventDto));

    readEventFromFile->_syncWithFileTable(syncroFileDataAndReader);
    readEventFromFile->onboardQueue();
}

void FileManager::_writeFile(const std::string &uri, ft::shared_ptr<HttpRequest> request) {

    FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
    FileData &fileData = fileTableManager.getFileData(uri);
    EventFactory &eventFactory = EventFactory::getInstance();

    ft::shared_ptr<SyncroFileDataAndWriter> syncroFileDataAndWriter = fileData.buildSyncroFileDataAndWriter();
    EventDto eventDto(request->getBody(), uri, "w");
    WriteEventToFile* writeEventToFile = static_cast<WriteEventToFile*>(eventFactory.createEvent(ft::FILE_WRITE_EVENT, eventDto));

    writeEventToFile->_syncWithFileTable(syncroFileDataAndWriter);
    writeEventToFile->onboardQueue();
}

std::string FileManager::getDirectoryListing(const std::string& path) {
    DIR *dir;
    struct dirent *ent;
    std::ostringstream oss;

    // HTML header
    oss << "<html><head><title>Directory Listing</title></head><body>";
    oss << "<h1>Directory listing for " << path << "</h1><hr><ul>";

    if ((dir = opendir(path.c_str())) != NULL) {
        // print all the files and directories within directory
        while ((ent = readdir(dir)) != NULL) {
            if(ent->d_type == DT_DIR) {
                // It's a directory, display with a trailing slash
                oss << "<li><a href=\"" << ent->d_name << "/\">" << ent->d_name << "/</a></li>";
            } else {
                // It's a file, display it normally
                oss << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a></li>";
            }
        }
        closedir (dir);
    } else {
        // Could not open directory
        oss << "Could not open directory.";
    }

    // HTML footer
    oss << "</ul><hr></body></html>";

    return oss.str();
}

e_file_info FileManager::getFileInfo(const std::string &uri, struct stat &fileStat) {
    if (stat(uri.c_str(), &fileStat) < 0) {
        return (NotExistFile);
    }
    if (S_ISDIR(fileStat.st_mode)) {
        return (ExistDirectory);
    }
    return (ExistFile);
}

// 항상 getFileinfo를 통해서 fileStat을 받아와야됨
bool FileManager::isInCashSize(struct stat &fileStat) {
    if (fileStat.st_size < Cache::cache_block_size) {
        return (true);
    }
    return (false);
}

bool FileManager::isInCashSize(size_t size) {
    if (size <= Cache::cache_block_size) {
        return (true);
    }
    return (false);
}

size_t FileManager::getFileSize(const std::string &uri) {
    struct stat fileStat;
    e_file_info fileInfo = getFileInfo(uri, fileStat);

    if (fileInfo == NotExistFile) {
        return (0);
    }
    return (fileStat.st_size);
}

bool FileManager::isFileExists(const std::string &uri) {
    struct stat fileStat;
    e_file_info fileInfo = getFileInfo(uri, fileStat);

    if (fileInfo == NotExistFile) {
        return (false);
    }
    return (true);
}

bool FileManager::isDirectory(const std::string &uri) {
    struct stat fileStat;
    e_file_info fileInfo = getFileInfo(uri, fileStat);

    if (fileInfo == ExistDirectory) {
        return (true);
    }
    return (false);
}
// check FileSuccess with responseType setting
// modulizing Need
// 이 전에 헤더 내용이 buffer에 들어가있다라고 가정하기떄문에 파일 사이즈 물어보는게 있어서 수정하고 들어가있는걸 가정
// check FileSuccess with response->statusCode setting
e_FileRequestType FileManager::requstFileContent(const std::string &uri, ft::shared_ptr<HttpResponse> response) {
    struct stat fileStat;
    e_file_info fileInfo = getFileInfo(uri, fileStat);

    e_File_Sync fileSync = response->getFileSync(HttpResponse::AccessKey());
    // can't here, buildHeader check this
    if (fileSync == NotSetting) {

        if (fileInfo == NotExistFile) {
            throw NotFoundException();
            return (FileRequestFail);
        }
        if (fileInfo == ExistDirectory) {
            throw std::runtime_error("FileManager::requstFileContent() : ExistDirectory, Logically Not Possible");
        }
    }
    if (isInCashSize(fileStat)) {
        Cache & cache = Cache::getInstance();
        //cache hit은 파일이 완전히 업로드 되야 hi
        if (cache.hit(uri)) {
            cache.copyCacheContentVectorBack(uri, response->getNormalCaseBuffer(HttpResponse::AccessKey()));
            response->setResponseSize(NormalSize, HttpResponse::AccessKey());
            return (FileRequestSuccess);
        }
        else {
            if (fileSync == NotSetting) {
                // cache에 없지만 cache사이즈 인건데 파일이 존재하는 경우-> 등록하고 기다려야됨
                cache.initCacheContent(uri);
                response->setFileSync(Reading, HttpResponse::AccessKey());
            }
            //else 는 파일이 등록 되고 있는중이라 hit안된거임으로 파일을 등록할 필요 없음
            return (FileRequestShouldWait);
        }
    }
    else {
        FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
        e_FileProcessingType fileProcessingType = fileTableManager.findFileProcessingType(uri);

        if (fileSync == ReadingDone) {
            return (FileRequestSuccess);
        }
        if (fileProcessingType == ReadingProcessing || fileProcessingType == NoneProcessing) {
            if (fileSync == NotSetting) {
                response->setResponseSize(BigSize, HttpResponse::AccessKey());
                response->setFileSync(Reading, HttpResponse::AccessKey());
                _readFile(uri, response);
            }
            //else 는 파일이 이 읽히고 있는중이고 그리고 따로 readFile에서 자동으로 ReadingProcessing으로 바꿀거라 ㄱㅊ
            return (FileRequestShouldWait);
        }
        else
            //file 이 수정되고 나서의 상태 예측이 불가능하기 떄문에 이와 같이 진행
            return (FileRequestShouldWait);
    }
}





e_FileRequestType FileManager::requestFileUpload(const std::string &uri,
ft::shared_ptr<HttpRequest> request) {
    if (request->getBodyType() == MULTIPART_FORM_DATA) {
        return (_requestFileUploadMultiPart(uri, request));
    }
    else {
        return (_requestFileUploadDefault(uri, request));
    }
}

e_FileRequestType FileManager::_requestFileUploadMultiPart(const std::string &uri,
ft::shared_ptr<HttpRequest> request) {
    //Todo: 
    (void)uri;
    (void)request;
    return (FileRequestFail);
}

e_FileRequestType FileManager::_requestFileUploadDefault(const std::string &uri,
ft::shared_ptr<HttpRequest> request) {
    FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
    //fileTableManager 에 없으면 fileData를 만들어서 넣어줌 NoneProcessing으로
    e_FileProcessingType fileProcessingType = fileTableManager.findFileProcessingType(uri);
    size_t contentLength = request->getBodySize();
    e_file_upload_sync fileUploadSync = request->getFileUploadSync(HttpRequest::AccessKey());
    Cache &cache = Cache::getInstance();

    if (fileProcessingType == ReadingProcessing) {
        return (FileRequestShouldWait);
    }
    else if (fileProcessingType == WritingProcessing) {
        // 자기가 쓰고 있던 남의 쓰고 있던 기다려야됨 
        return (FileRequestShouldWait);
    }
    else if (fileProcessingType == NoneProcessing) {
        // 자기가 쓴게 완료된 경우 처리부터
        if (fileUploadSync == Writing) {
            request->setFileUploadSync(WritingDone, HttpRequest::AccessKey());
            return (FileRequestSuccess);
        }
        // 남이 쓴게 완료한 경우 처리부터 -> cash에 있는 파일인지, 아닌지에 따라 다름
        //여기서 부터는 캐쉬에 있는건데 더 커지는거 더 작아지는것도 고려해야됨

        if (fileUploadSync == NoneSetting) {
            //cache에 있는건데 쓰려는 경우
            if (cache.hit(uri)) {
                if (!isInCashSize(contentLength)){
                    cache.deleteCacheContent(uri);
                    _writeFile(uri, request);
                    request->setFileUploadSync(Writing, HttpRequest::AccessKey());
                    return (FileRequestShouldWait);
                }
                else {
                    cache.putCacheContent(uri, request->getBody());
                    request->setFileUploadSync(WritingDone, HttpRequest::AccessKey());
                    return (FileRequestSuccess);
                }
            }
            if (isInCashSize(contentLength)){
                cache.putCacheContent(uri, request->getBody());
                request->setFileUploadSync(WritingDone, HttpRequest::AccessKey());
                return (FileRequestSuccess);
            }
            else {
                _writeFile(uri, request);
                request->setFileUploadSync(Writing, HttpRequest::AccessKey());
                return (FileRequestShouldWait);
            }
        }
    }
    return (FileRequestFail);
}

e_FileRequestType FileManager::requestFileDelete(const std::string &uri) {
   struct stat fileStat;
    e_file_info fileInfo = getFileInfo(uri, fileStat);
    if (fileInfo == NotExistFile) {
            throw NotFoundException();
        }
    if (fileInfo == ExistDirectory) {
            throw ForbiddenException();
        }
    Cache &cache = Cache::getInstance();
    if (cache.hit(uri)) {
        cache.deleteCacheContent(uri);
        return (FileRequestSuccess);
    }
    FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
    e_FileProcessingType fileProcessingType = fileTableManager.findFileProcessingType(uri);
    if (fileProcessingType == ReadingProcessing) {
        return (FileRequestShouldWait);
    }
    else if (fileProcessingType == WritingProcessing) {
        return (FileRequestShouldWait);
    }
    else if (fileProcessingType == NoneProcessing) {
        fileTableManager.deleteFileData(uri);
        unlink(uri.c_str());
        return (FileRequestSuccess);
    }
    return (FileRequestFail);
}