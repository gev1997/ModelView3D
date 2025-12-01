#include "ModelView3D/model_loader.h"

#include <igl/readSTL.h>
#include <igl/readOBJ.h>

#include <iostream>
#include <fstream>

std::optional<model_data> model_loader::load(const fs::path& path)
{
    // TODO: empth and valid path check
    model_data data;

    std::ifstream input(path.string(), std::ios::binary);  // Binary mode for robustness with binary STLs
    if (!input) {
        std::cerr << "Failed to open 3D model: " << path << std::endl;
        return std::nullopt;
    }

    // Read STL
    bool ok = igl::readSTL(input, data.V, data.F, data.N);
    if (!ok)
    {
        std::cerr << "Failed to load 3D model" << std::endl;
        return std::nullopt;
    }

    return data;
}
