#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <Hash/sha256.h>

void generate_etag(const std::string& filepath, std::string& etag) {
    struct stat file_stat;
    SHA256_CTX ctx;
    BYTE hash[SHA256_BLOCK_SIZE];

    if (stat(filepath.c_str(), &file_stat) != 0) {
        perror("stat");
        return;
    }

    sha256_init(&ctx);

    // Prepare metadata
    std::ostringstream oss;
    oss << filepath << "_" << file_stat.st_size << "_" << file_stat.st_mtime;

    // Update hash with metadata
    std::string metadata = oss.str();
    sha256_update(&ctx, (const BYTE*)metadata.c_str(), metadata.size());

    sha256_final(&ctx, hash);

    // Convert hash to hexadecimal string
    std::ostringstream etag_stream;
    for (int i = 0; i < SHA256_BLOCK_SIZE; ++i) {
        etag_stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    etag = etag_stream.str();
}

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         std::cerr << "Usage: " << argv[0] << " <filepath>" << std::endl;
//         return 1;
//     }

//     std::string etag;
//     generate_etag(argv[1], etag);
//     std::cout << "ETag: " << etag << std::endl;

//     return 0;
// }
