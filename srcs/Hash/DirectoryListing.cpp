#include <dirent.h>
#include <sstream>
#include <string>
#include <iostream>

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


// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <directory path>" << std::endl;
//         return 1;
//     }

//     std::string directory_listing = getDirectoryListing(argv[1]);
//     std::cout << directory_listing;

//     return 0;
// }
