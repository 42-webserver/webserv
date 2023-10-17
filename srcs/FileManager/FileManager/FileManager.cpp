#include <FileManager/FileManager/FileManger.hpp>

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
    //Todo :: fileSync로 자동으로 readerCount증가하는거 생각 
    // kqueue에 등록해야됨 readFile
    // File*로 파일 새로 열어줌 ( 같은 파일에 대해 여러개 여는중)
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

std::string getDirectoryListing(const std::string& path) {
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
    if (size < Cache::cache_block_size) {
        return (true);
    }
    return (false);
}
//check FileSuccess with responseType setting
// modulizing Need
// 이 전에 헤더 내용이 buffer에 들어가있다라고 가정하기떄문에 파일 사이즈 물어보는게 있어서 수정하고 들어가있는걸 가정
// check FileSuccess with response->statusCode setting
// e_FileRequestType FileManager::requstFileContent(const std::string &uri, ft::shared_ptr<HttpResponse> response) {
//     struct stat fileStat;
//     e_file_info fileInfo = getFileInfo(uri, fileStat);

//     e_File_Sync fileSync = response->getFileSync(HttpResponse::AccessKey());
//     if (fileSync == NotSetting) {

//         if (fileInfo == NotExistFile) {
//             response->setStatusCode(NOT_FOUND);
//             return (FileRequestFail);
//         }
//         if (fileInfo == ExistDirectory) {
//             //To do: send directory listing Moduliziing
//             std::vector<char> &buffer = response->getNormalCaseBuffer(HttpResponse::AccessKey());
//             std::string directoryListing = getDirectoryListing(uri);

//             buffer.insert(buffer.end(), directoryListing.begin(), directoryListing.end());
//             response->setResponseSize(normalSize, HttpResponse::AccessKey());
//             response->setStatusCode(OK);
//             return (FileRequestSuccess);
//         }
//     }
//     if (isInCashSize(fileStat)) {
//         Cache & cache = Cache::getInstance();
//         if (cache.hit(uri)) {
//             cache.getCacheContent(uri, response->getNormalCaseBuffer(HttpResponse::AccessKey()));
//             response->setResponseSize(normalSize, HttpResponse::AccessKey());
//             response->setStatusCode(OK);
//             return (FileRequestSuccess);
//         }
//         else {
//             if (fileSync == NotSetting) {
//                 // 이 밑에 코드에서는 파일을 등록을 하는데 cache가 등록하도록해야됨 
//                 cache.putCacheContent(uri);
//                 response->setFileSync(Reading, HttpResponse::AccessKey());
//             }
//             //else 는 파일이 등록 되고 있는중이라 hit안된거임으로 파일을 등록할 필요 없음
//             return (FileRequestShouldWait);
//         }
//     }
//     else {
//         FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
//         e_FileProcessingType fileProcessingType = fileTableManager.findFileProcessingType(uri);

//         if (fileSync == ReadingDone) {
//             response->setStatusCode(OK);
//             return (FileRequestSuccess);
//         }
//         if (fileProcessingType == ReadingProcessing || fileProcessingType == NoneProcessing) {
//             if (fileSync == NotSetting) {
//                 response->setResponseSize(BigSize, HttpResponse::AccessKey());
//                 response->setFileSync(Reading, HttpResponse::AccessKey());
//                 _readFile(uri, response);
//             }
//             //else 는 파일이 이 읽히고 있는중이고 그리고 따로 readFile에서 자동으로 ReadingProcessing으로 바꿀거라 ㄱㅊ
//             return (FileRequestShouldWait);
//         }
//         else
//             //file 이 수정되고 나서의 상태 예측이 불가능하기 떄문에 이와 같이 진행
//             return (FileRequestShouldWait);
//     }
// }


e_FileRequestType FileManager::requestFileUpload(const std::string &uri,
ft::shared_ptr<HttpRequest> request) {
    if (request->getBodyType() == MULTIPART_FORM_DATA) {
        return (_requestFileUploadMultiPart(uri, request));
    }
    else {
        return (_requestFileUploadDefault(uri, request));
    }
}

e_FileRequestType FileManager::_requestFileUploadDefault(const std::string &uri,
ft::shared_ptr<HttpRequest> request) {
//Todo: logic
    FileTableManager &fileTableManager = FileTableManager::getInstance(FileTableManager::Accesskey());
    //fileTableManager 에 없으면 fileData를 만들어서 넣어줌 NoneProcessing으로
    e_FileProcessingType fileProcessingType = fileTableManager.findFileProcessingType(uri);
    size_t contentLength = request->getBodySize();

    if (fileProcessingType == ReadingProcessing) {
        return (FileRequestShouldWait);
    }
    if (fileProcessingType == NoneProcessing) {
        if (isInCashSize(contentLength)) {
        }
        else {
        }
    }
    if (fileProcessingType == WritingProcessing) {
    }
}