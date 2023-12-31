#include "Loader.h"

#include <fstream>

#include "nlohmann/json.hpp"

namespace wind::io
{
    nlohmann::json LoadJson(std::filesystem::path filePath) {
        using json = nlohmann::json;
        json j;
        std::ifstream jfile(filePath);
        jfile >> j;
        return j;
    }
}