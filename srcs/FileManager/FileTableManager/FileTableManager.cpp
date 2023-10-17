#include <FileManager/FileTableManager/FileTableManager.hpp>

FileTableManager::Accesskey::Accesskey(void) {}
FileTableManager::Accesskey::~Accesskey(void) {}

FileTableManager *FileTableManager::_instance = NULL;

FileTableManager::FileTableManager(void) {}

FileTableManager::~FileTableManager(void) {}

FileTableManager &FileTableManager::getInstance(Accesskey)
{
    if (FileTableManager::_instance == NULL)
        FileTableManager::_instance = new FileTableManager();
    return (*FileTableManager::_instance);
}


// 이미 파일이 존재하는지 확인한 후, 물어보는 동작을 가정
// 없으면 만들어서 파일테이블에 새로 올려야됨 -> 파일테이블에 올리는 동작을 가정
// 생성할떈 fileProcessingType을 None으로 해야됨
e_FileProcessingType FileTableManager::findFileProcessingType(const std::string &fileName)
{
    std::map<std::string, FileData>::iterator iter = this->_fileTable.find(fileName);
    if (iter == this->_fileTable.end())
        this->_fileTable.insert(std::make_pair(fileName, Fil₩eData())); 
    return (_fileTable[fileName].getFileProcessingType());
}

// 위와 거의 동일
FileData &FileTableManager::getFileData(const std::string &fileName)
{
    std::map<std::string, FileData>::iterator iter = this->_fileTable.find(fileName);
    if (iter == this->_fileTable.end())
        this->_fileTable.insert(std::make_pair(fileName, FileData())); 
    return (_fileTable[fileName]);
}