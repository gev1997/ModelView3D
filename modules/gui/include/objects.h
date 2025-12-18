#pragma once

// project
#include "namespace_gui.h"

// third party
#include <glad/glad.h>

// std
#include <vector>

class gui::gl_object
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
class gui::gl_buffer : public gl_object
{
public:
    gl_buffer(GLenum target) : m_target(target) {}
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
class gui::vertex_buffer_object : public gl_buffer<GLfloat>
{
public:
    vertex_buffer_object() : gl_buffer{GL_ARRAY_BUFFER} {}
};

// EBO
class gui::element_buffer_object : public gl_buffer<GLuint>
{
public:
    element_buffer_object() : gl_buffer{GL_ELEMENT_ARRAY_BUFFER} {}
};

// VAO
class gui::vertex_array_object : public gl_object
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
