#pragma once

// project

// third party
#include <Eigen/Core>

// std
#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

namespace raw_model
{

struct data
{
	Eigen::MatrixXd V; // vertices, size (#V x 3)
	Eigen::MatrixXi F; // faces (triangles), size (#F x 3)
	Eigen::MatrixXd N; // per-face or per-vertex normals, size (#X x 3)
};

class loader
{
public:
	std::optional<data> load(const fs::path& path);
};

};
