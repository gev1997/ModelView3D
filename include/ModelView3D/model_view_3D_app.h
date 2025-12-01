#pragma once

#include "mesh.h"
#include "shader.h"
#include "viewer.h"
#include "ModelView3D/model_loader.h"

class model_view_3D_app
{
public:
	model_view_3D_app() = default;
	~model_view_3D_app();
	bool init();
    bool load_model(const fs::path& path);
	void main_loop();

private:
	void _init_shaders();
	void _init_mesh();

private:
	GLFWwindow* m_main_window = nullptr; // TODO: use unique_ptr with custom deleter (glfwDestroyWindow)
	std::unique_ptr<viewer> m_viewer = nullptr;
	std::unique_ptr<shader_program> m_shader_program = nullptr;
    // mesh m;
    model_data m_model_data;
    fs::path m_current_model;
	static constexpr auto TITLE{"ModelView3D"};
	static constexpr auto WIDTH{800};
	static constexpr auto HEIGHT{800};
};
