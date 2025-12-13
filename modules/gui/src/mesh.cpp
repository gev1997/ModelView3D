// project
#include "mesh.h"

// third party

// std

gui::mesh::mesh(raw_model::data data)
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
