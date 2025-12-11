#pragma once

// project
#include "raw_model.h"

// third party
#include <glad/glad.h>

// std
#include <vector>

class gl_object
{
public:
    gl_object() = default;
    virtual ~gl_object() = default;

    GLuint get_ID() const { return m_ID; }

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual void destroy() const = 0;

protected:
    GLuint m_ID = 0;
};

template <typename T>
class gl_buffer : public gl_object
{
public:
    gl_buffer(const GLenum target) : m_target(target) {}
    virtual ~gl_buffer() = default;

    void init(const std::vector<T>& data)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(m_target, m_ID);

        const auto size = static_cast<GLsizeiptr>(data.size() * sizeof(T));
	    glBufferData(m_target, size, data.data(), GL_STATIC_DRAW);  
    }

    void bind() const override
    {
        glBindBuffer(m_target, m_ID);
    }

    void unbind() const override
    {
        glBindBuffer(m_target, 0);
    }

    virtual void destroy() const override
    {
        glDeleteBuffers(1, &m_ID);
    }

protected:
    GLenum m_target;
};

// VBO
class vertex_buffer_object : public gl_buffer<GLfloat>
{
public:
    vertex_buffer_object() : gl_buffer{GL_ARRAY_BUFFER} {}
};

// EBO
class element_buffer_object : public gl_buffer<GLuint>
{
public:
    element_buffer_object() : gl_buffer{GL_ELEMENT_ARRAY_BUFFER} {}
};

// VAO
class vertex_array_object : public gl_object
{
public:
    vertex_array_object()
    {
	    glGenVertexArrays(1, &m_ID);
    }

    virtual ~vertex_array_object() = default;

    void link_vertex_buffer_object(const vertex_buffer_object& VBO, GLuint layout, GLint size, GLenum type, GLsizei stride, void* offset)
    {
        VBO.bind();
        glVertexAttribPointer(layout, size, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        VBO.unbind();
    }

    void bind() const override
    {
        glBindVertexArray(m_ID);
    }

    void unbind() const override
    {
        glBindVertexArray(0);
    }

    void destroy() const override
    {
        glDeleteVertexArrays(1, &m_ID);
    }
};

class mesh
{
public:
    mesh(raw_model::data data)
    {
        Eigen::Vector3d min = data.V.colwise().minCoeff();
        Eigen::Vector3d max = data.V.colwise().maxCoeff();
        Eigen::Vector3d center = (min + max) / 2.0;
        const double size = (max - min).maxCoeff();           // longest dimension

        // Translate model to origin
        for (int i = 0; i < data.V.rows(); ++i)
            data.V.row(i) -= center.transpose();

        // Now scale so that longest side = 2.0 units (perfect for camera at z = -3 to -5)
        const double scale = (size > 0.0001) ? 2.0 / size : 1.0;
        for (int i = 0; i < data.V.rows(); ++i)
            data.V.row(i) *= scale;

        for (int f = 0; f < data.F.rows(); ++f)
        {
            Eigen::Vector3f n = (data.N.rows() == data.F.rows())
                                ? data.N.row(f).cast<GLfloat>()
                                : Eigen::Vector3f(0,1,0);

            for (int j = 0; j < 3; ++j)
            {
                const Eigen::Vector3f v = data.V.row(data.F(f,j)).cast<GLfloat>();

                m_vertices.push_back(v.x()); m_vertices.push_back(v.y()); m_vertices.push_back(v.z());
                m_vertices.push_back(n.x()); m_vertices.push_back(n.y()); m_vertices.push_back(n.z());
            }

            const auto base = static_cast<GLuint>(m_indices.size());
            m_indices.push_back(base + 0);
            m_indices.push_back(base + 1);
            m_indices.push_back(base + 2);
        }

        m_VAO.bind();
        m_VBO.init(m_vertices);
        m_EBO.init(m_indices);
        m_EBO.bind();
        m_VAO.link_vertex_buffer_object(m_VBO, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0);
        m_VAO.link_vertex_buffer_object(m_VBO, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

        m_VAO.unbind();
        m_VBO.unbind();
        m_EBO.unbind();
    }

    ~mesh() = default;
    
    void destroy()
    {
        m_VAO.destroy();
        m_VBO.destroy();
        m_EBO.destroy();
    }

    const vertex_array_object& get_VAO() const
    {
        return m_VAO;
    }

    std::size_t get_indices_size()
    {
        return m_indices.size();
    }

private:
    vertex_array_object m_VAO;
    vertex_buffer_object m_VBO;
    element_buffer_object m_EBO;
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
};
