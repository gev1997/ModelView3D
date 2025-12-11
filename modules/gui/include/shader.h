#pragma once

// project

// third party
#include <glad/glad.h>

// std
#include <filesystem>

namespace fs = std::filesystem;

class shader_base
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

class vertex_shader : public shader_base
{
public:
	vertex_shader(const fs::path& path);
	virtual ~vertex_shader() = default;
};

class fragment_shader : public shader_base
{
public:
	fragment_shader(const fs::path& path);
	virtual ~fragment_shader() = default;
};

class shader_program
{
public:
	shader_program(const fs::path& shader_path);
	~shader_program();

	void attach() const;
	void detach() const;
	void destroy();
	GLuint get() const;
	void set_matrix4(const std::string& name, const float* mat) const;
	
private:
	GLuint m_program_ID = 0;
};
