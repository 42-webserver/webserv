#ifndef FILE_NAME_GENERATOR_HPP
#define FILE_NAME_GENERATOR_HPP

#include <string>
#include <sstream>
#include <ctime>
#include <fstream>

static const std::string FILE_LOCATION = "/tmp/";

class FileNameGenerator
{
public:
	static std::string generateUniqueFileName();

private:
	static bool isFileExists(const std::string& filename);
};

#endif