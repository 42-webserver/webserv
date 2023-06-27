#include "../../../incs/http/parser/FileNameGenerator.hpp"

bool FileNameGenerator::isFileExists(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    return file.good();
}

std::string FileNameGenerator::generateUniqueFileName()
{
    std::stringstream filenameStream;
    
    std::time_t currentTime = std::time(NULL);
    filenameStream << "file_" << currentTime << ".txt";
    
    std::string filename = filenameStream.str();
    while (isFileExists(FILE_LOCATION + filename))
    {
        currentTime++;
        filenameStream.str("");
        filenameStream << "file_" << currentTime << ".txt";
        filename = filenameStream.str();
    }
    
    return FILE_LOCATION + filename;
}