
#ifndef UTILS 
#define UTILS 

#include <vector>
#include <string>
#include <filesystem>


namespace Utils{
    std::vector<std::string> getFilesInFolder(std::filesystem::path path);
}


#endif