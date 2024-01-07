
#include "utils.h"
#include <vector>


namespace Utils {

    std::vector<std::string> getFilesInFolder(std::filesystem::path path){
        
        std::vector<std::string> output;

        for (const auto & entry : std::filesystem::directory_iterator(path)){

            if (entry.is_directory()){
                getFilesInFolder(entry.path());
            }else {
                output.push_back(entry.path().string());
            }

        }


        return output;
    }

}

