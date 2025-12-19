// project
#include "model_view_3D_app.h"

// third party
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <iostream>
#include <cassert>

gui::model_view_3D_app::~model_view_3D_app()
{
	assert(m_main_window);
	assert(m_viewer);
	assert(m_shader_program);
	assert(m_mesh);

	m_mesh->destroy();
	m_shader_program->destroy();
	glfwDestroyWindow(m_main_window);
	glfwTerminate();
}

bool gui::model_view_3D_app::init()
{
	assert(!m_main_window);

	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_main_window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
	if (!m_main_window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		return false;
	}

	glfwMakeContextCurrent(m_main_window);

	if (!gladLoadGL())
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(m_main_window, [](GLFWwindow* win, int w, int h){
        glViewport(0, 0, w, h);
    });

	m_viewer = std::make_unique<viewer>(m_main_window);
	m_shader_program = std::make_unique<shader_program>("basic");

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(m_main_window);
	glfwSwapInterval(1);

	return true;
}

bool gui::model_view_3D_app::load_model(const fs::path& path)
{
	raw_model::loader model_loader;
	const auto data = model_loader.load(path);

	if (!data)
		return false;

	m_mesh = std::make_unique<mesh>(*data);

	return true;
}

void gui::model_view_3D_app::main_loop()
{
	assert(m_main_window);
	assert(m_viewer);
	assert(m_shader_program);
	assert(m_mesh);

	m_shader_program->attach();
	m_mesh->get_VAO().bind();
	
	auto& camera = m_viewer->get_camera();
	const auto indices_size = m_mesh->get_indices_size();
	
	while (!glfwWindowShouldClose(m_main_window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_shader_program->set_vec3("lightPos",  {2.0f, 2.0f, 2.0f});
		m_shader_program->set_vec3("viewPos",   camera.get_position());
		m_shader_program->set_vec3("objectColor", {0.8f, 0.8f, 0.8f});

		m_shader_program->set_matrix4("model", camera.get_model_matrix());
		m_shader_program->set_matrix4("view", camera.get_view_matrix());
		m_shader_program->set_matrix4("projection", camera.get_projection_matrix());

		glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(m_main_window);
		glfwPollEvents();
	}
}
