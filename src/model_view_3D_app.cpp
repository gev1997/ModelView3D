#include "ModelView3D/model_view_3D_app.h"
// #include "ModelView3D/model_loader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

model_view_3D_app::~model_view_3D_app()
{
	assert(m_main_window);

	glfwDestroyWindow(m_main_window);
	glfwTerminate();
}

bool model_view_3D_app::init()
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
	_init_shaders();
	// _init_mesh();

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(m_main_window);
	glfwSwapInterval(1);

	return true;
}

bool model_view_3D_app::load_model(const fs::path& path)
{
	model_loader model_loader;
	auto data = model_loader.load(path);

	if (!data)
		return false;

	// _init_mesh();
	m_model_data = *data;

	return true;
}

void model_view_3D_app::main_loop()
{
	assert(m_main_window);

	Eigen::Vector3d min = m_model_data.V.colwise().minCoeff();
	Eigen::Vector3d max = m_model_data.V.colwise().maxCoeff();
	Eigen::Vector3d center = (min + max) / 2.0;
	double size = (max - min).maxCoeff();           // longest dimension

	// Translate model to origin
	for (int i = 0; i < m_model_data.V.rows(); ++i)
		m_model_data.V.row(i) -= center.transpose();

	// Now scale so that longest side = 2.0 units (perfect for camera at z = -3 to -5)
	double scale = (size > 0.0001) ? 2.0 / size : 1.0;
	for (int i = 0; i < m_model_data.V.rows(); ++i)
    m_model_data.V.row(i) *= scale;

    std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	for (int f = 0; f < m_model_data.F.rows(); ++f)
	{
		Eigen::Vector3f n = (m_model_data.N.rows() == m_model_data.F.rows())
			? m_model_data.N.row(f).cast<float>()
			: Eigen::Vector3f(0,1,0);

		for (int j = 0; j < 3; ++j)
		{
			Eigen::Vector3f v = m_model_data.V.row(m_model_data.F(f,j)).cast<float>();

			vertices.push_back(v.x()); vertices.push_back(v.y()); vertices.push_back(v.z());
			vertices.push_back(n.x()); vertices.push_back(n.y()); vertices.push_back(n.z());
		}
		unsigned int base = (unsigned int)indices.size();
		indices.push_back(base+0);
		indices.push_back(base+1);
		indices.push_back(base+2);
	}

	vertex_array_object VAO;
	VAO.bind();

	vertex_buffer_object VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
	element_buffer_object EBO(indices.data(), indices.size() * sizeof(GLuint));
	EBO.bind();

	VAO.link_vertex_buffer_object(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO.link_vertex_buffer_object(VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO.unbind();
	VBO.unbind();
	EBO.unbind();

	while (!glfwWindowShouldClose(m_main_window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader_program->attach();
		m_viewer->get_camera().update_uniforms(m_shader_program.get());

		VAO.bind();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(m_main_window);
		glfwPollEvents();
	}
}

void model_view_3D_app::_init_shaders()
{
	m_shader_program = std::make_unique<shader_program>("basic");
}

void model_view_3D_app::_init_mesh() // can be deleted by RAII
{
	
}
