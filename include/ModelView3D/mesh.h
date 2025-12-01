#pragma once

#include<glad/glad.h>

class vertex_buffer_object;

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
    gl_buffer(GLenum target, T* data, GLsizeiptr size)
        : m_target(target)
    {
        glGenBuffers(1, &m_ID);
        glBindBuffer(m_target, m_ID);
	    glBufferData(m_target, size, data, GL_STATIC_DRAW);
    }

    virtual ~gl_buffer() = default;

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
    vertex_buffer_object(GLfloat* data, GLsizeiptr size)
        : gl_buffer{GL_ARRAY_BUFFER, data, size}
    {}
};

// EBO
class element_buffer_object : public gl_buffer<GLuint>
{
public:
    element_buffer_object(GLuint* data, GLsizeiptr size)
        : gl_buffer{GL_ELEMENT_ARRAY_BUFFER, data, size}
    {}
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
