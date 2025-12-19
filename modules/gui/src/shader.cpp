// project
#include "shader.h"

// third party

// std
#include <fstream>
#include <sstream>

gui::shader_base::shader_base(int shader_ID, const fs::path& path)
{
	const auto shader_source = _get_shader_source(path);
	const GLchar* shader_source_cstr = shader_source.c_str();

	m_shader = glCreateShader(shader_ID);
	glShaderSource(m_shader, 1, &shader_source_cstr, NULL);
	glCompileShader(m_shader);
}

gui::shader_base::~shader_base()
{
	glDeleteShader(m_shader);
}

GLuint gui::shader_base::get() const
{
	return m_shader;
}

std::string gui::shader_base::_get_shader_source(const fs::path& path)
{
	std::ifstream file(path);
    if (!file)
		return {};

    std::stringstream stream;
    stream << file.rdbuf();

	return stream.str();
}

gui::vertex_shader::vertex_shader(const fs::path& path)
	: shader_base{GL_VERTEX_SHADER, path / "vertex.glsl"}
{}

gui::fragment_shader::fragment_shader(const fs::path& path)
	: shader_base{GL_FRAGMENT_SHADER, path / "fragment.glsl"}
{}

gui::shader_program::shader_program(const fs::path& path)
{
	const fs::path shaders_path = "shaders" / path;
	vertex_shader vertex_shader(shaders_path);
	fragment_shader fragment_shader(shaders_path);

	m_program_ID = glCreateProgram();
	glAttachShader(m_program_ID, vertex_shader.get());
	glAttachShader(m_program_ID, fragment_shader.get());
	glLinkProgram(m_program_ID);
}

gui::shader_program::~shader_program()
{
	destroy();
}

void gui::shader_program::attach() const
{
	if (!m_program_ID)
		return;

	glUseProgram(m_program_ID);
}

void gui::shader_program::detach() const
{
    glUseProgram(0);
}

void gui::shader_program::destroy()
{
	if (!m_program_ID)
		return;

	glDeleteProgram(m_program_ID);
	m_program_ID = 0;
}

GLuint gui::shader_program::get() const
{
	return m_program_ID;
}

void gui::shader_program::set_vec3(const std::string& name, const Eigen::Vector3f& vec3) const
{
    const GLint location = glGetUniformLocation(m_program_ID, name.c_str());
	glUniform3fv(location, 1, vec3.data());
}

void gui::shader_program::set_matrix4(const std::string& name, const Eigen::Matrix4f& mat4) const
{
	const GLint location = glGetUniformLocation(m_program_ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, mat4.data());
}
