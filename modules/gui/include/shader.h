#pragma once

// project
#include "namespace_gui.h"

// third party
#include <Eigen/Geometry>
#include <glad/glad.h>

// std
#include <filesystem>

namespace fs = std::filesystem;

class gui::shader_base
{
public:
	shader_base(int shader_ID, const fs::path& path);
	virtual ~shader_base();

	GLuint get() const;

private:
    std::string _get_shader_source(const fs::path& path);

private:
	GLuint m_shader;
};

class gui::vertex_shader : public shader_base
{
public:
	vertex_shader(const fs::path& path);
	virtual ~vertex_shader() = default;
};

class gui::fragment_shader : public shader_base
{
public:
	fragment_shader(const fs::path& path);
	virtual ~fragment_shader() = default;
};

class gui::shader_program
{
public:
	shader_program(const fs::path& shader_path);
	~shader_program();

	void attach() const;
	void detach() const;
	void destroy();
	GLuint get() const;
	void set_vec3(const std::string& name, const Eigen::Vector3f& vec3) const;
	void set_matrix4(const std::string& name, const Eigen::Matrix4f& mat4) const;
	
private:
	GLuint m_program_ID = 0;
};
