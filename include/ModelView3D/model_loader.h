#pragma once

#include <Eigen/Core>

#include <filesystem>
#include <optional>

namespace fs = std::filesystem;

struct model_data
{
	Eigen::MatrixXd V; // vertices, size (#V x 3)
	Eigen::MatrixXi F; // faces (triangles), size (#F x 3)
	Eigen::MatrixXd N; // per-face or per-vertex normals, size (#X x 3)
};

class model_loader
{
public:
	std::optional<model_data> load(const fs::path& path);
};
