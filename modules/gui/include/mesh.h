#pragma once

// project
#include "namespace_gui.h"
#include "raw_model.h"
#include "objects.h"

// third party

// std

class gui::mesh
{
public:
    mesh(raw_model::data data);
    ~mesh() = default;
    
    const vertex_array_object& get_VAO() const;
    std::size_t get_indices_size();
    void destroy();

private:
    vertex_array_object m_VAO;
    vertex_buffer_object m_VBO;
    element_buffer_object m_EBO;
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
};
