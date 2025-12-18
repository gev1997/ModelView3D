// project
#include "mesh.h"

// third party

// std
#include <numeric>

gui::mesh::mesh(raw_model::data data)
{
    auto& vertices = data.V;
    auto& faces = data.F;
    auto& normals = data.N;

    Eigen::Vector3d min = vertices.colwise().minCoeff();
    Eigen::Vector3d max = vertices.colwise().maxCoeff();
    Eigen::Vector3d center = (min + max) / 2.0;
    const double size = (max - min).maxCoeff();
    const double scale = (size > 0.0001) ? 2.0 / size : 1.0;

    for (int i = 0; i < vertices.rows(); ++i)
    {
        vertices.row(i) -= center.transpose();
        vertices.row(i) *= scale;
    }

    for (int f = 0; f < faces.rows(); ++f)
    {
        const Eigen::Vector3f normal = (normals.rows() == faces.rows())
                                       ? normals.row(f).cast<GLfloat>() // per-face
                                       : Eigen::Vector3f(0,1,0);        // per-vertex

        for (int j = 0; j < 3; ++j)
        {
            const Eigen::Vector3f vertex = vertices.row(faces(f,j)).cast<GLfloat>();

            m_vertices.insert(m_vertices.end(), {vertex.x(), vertex.y(), vertex.z()});
            m_vertices.insert(m_vertices.end(), {normal.x(), normal.y(), normal.z()});
        }
    }

    m_indices = std::vector<GLuint>(faces.size());
    std::iota(m_indices.begin(), m_indices.end(), 0);

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

const gui::vertex_array_object& gui::mesh::get_VAO() const
{
    return m_VAO;
}

std::size_t gui::mesh::get_indices_size()
{
    return m_indices.size();
}

void gui::mesh::destroy()
{
    m_VAO.destroy();
    m_VBO.destroy();
    m_EBO.destroy();
}
