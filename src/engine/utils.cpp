
#include "engine/utils.h"
#include <vector>


namespace Utils {

    std::vector<std::string> getFilesInFolder(std::filesystem::path path){
        
        std::vector<std::string> output;

        for (const auto & entry : std::filesystem::directory_iterator(path)){

            if (entry.is_directory()){
                for (auto other : getFilesInFolder(entry.path())){
                    output.push_back(other);
                }
            }else {
                output.push_back(entry.path().string());
            }

        }


        return output;
    }

}

