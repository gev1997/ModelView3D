#include "ModelView3D/model_loader.h"

#include <igl/readSTL.h>
#include <igl/readOBJ.h>

#include <iostream>
#include <fstream>

std::optional<raw_model::data> raw_model::loader::load(const fs::path& path)
{
    // TODO: empth and valid path check
    data data;

    std::ifstream input(path.string(), std::ios::binary);  // Binary mode for robustness with binary STLs
    if (!input) {
        std::cerr << "Failed to open 3D model: " << path << std::endl;
        return std::nullopt;
    }

    // Read STL
    if (igl::readSTL(input, data.V, data.F, data.N))
        return data;

    std::cerr << "Failed to load 3D model" << std::endl;
    return std::nullopt;
}
