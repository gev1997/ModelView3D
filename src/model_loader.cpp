#include "ModelView3D/model_loader.h"

#include <igl/readSTL.h>
#include <igl/readOBJ.h>

#include <iostream>
#include <fstream>

std::optional<raw_model::data> raw_model::loader::load(const fs::path& path)
{
    if (!fs::exists(path))
    {
        std::cerr << "Invalid 3D model path: " << path << std::endl;
        return std::nullopt;
    }

    std::ifstream input(path.string(), std::ios::binary);
    if (!input)
    {
        std::cerr << "Failed to open 3D model: " << path << std::endl;
        return std::nullopt;
    }
    
    data data;
    
    if (!igl::readSTL(input, data.V, data.F, data.N))
    {
        std::cerr << "Failed to load 3D model" << std::endl;
        return std::nullopt;
    }

    return data;
}
