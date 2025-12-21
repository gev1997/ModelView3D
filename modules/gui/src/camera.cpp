// project
#include "camera.h"

// third party

// std
#include <cmath>

void gui::camera::process_mouse_move(float delta_x, float delta_y)
{
    m_yaw += delta_x * mouse_move_sensitivity;
    m_pitch -= delta_y * mouse_move_sensitivity;

    if (m_yaw > 2.0f * M_PI) m_yaw -= 2.0f * M_PI;
    if (m_yaw < 0.0f) m_yaw += 2.0f * M_PI;
    if (m_pitch > max_pitch) m_pitch = max_pitch;
    if (m_pitch < -max_pitch) m_pitch = -max_pitch;
}

void gui::camera::process_mouse_scroll(float offset_y)
{
    m_radius -= offset_y * 0.5f;
    if (m_radius < 0.1f)
        m_radius = 0.1f;
}

Eigen::Vector3f gui::camera::get_position() const
{
    const float x = m_radius * std::cos(m_pitch) * std::sin(m_yaw);
    const float y = m_radius * std::sin(m_pitch);
    const float z = m_radius * std::cos(m_pitch) * std::cos(m_yaw);

    return {x, y, z};
}

Eigen::Matrix4f gui::camera::get_view_matrix()
{
    Eigen::Vector3f eye = get_position();
    Eigen::Vector3f center = Eigen::Vector3f::Zero();
    Eigen::Vector3f world_up = Eigen::Vector3f::UnitY();

    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f s = f.cross(world_up).normalized();
    Eigen::Vector3f u = s.cross(f).normalized();

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    view.block<1,3>(0,0) = s.transpose();
    view.block<1,3>(1,0) = u.transpose();
    view.block<1,3>(2,0) = -f.transpose();
    view(0,3) = -s.dot(eye);
    view(1,3) = -u.dot(eye);
    view(2,3) = f.dot(eye);

    return view;
}

Eigen::Matrix4f gui::camera::get_projection_matrix()
{
    const float tan_half_field_of_view = std::tan(field_of_view * 0.008726646f);  // deg → rad / 2
    Eigen::Matrix4f projection = Eigen::Matrix4f::Zero();

    projection(0,0) = 1.0f / (aspect * tan_half_field_of_view);
    projection(1,1) = 1.0f / tan_half_field_of_view;
    projection(2,2) = -(z_far + z_near) / (z_far - z_near);
    projection(2,3) = -(2.0f * z_far * z_near) / (z_far - z_near);
    projection(3,2) = -1.0f;

    return projection;
}

Eigen::Matrix4f gui::camera::get_model_matrix()
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    if (!m_auto_spin)
        return model;

    m_model_angle += 0.3f;
    Eigen::AngleAxisf rotation(m_model_angle * 0.017453292f, Eigen::Vector3f::UnitY());
    model.block<3,3>(0,0) = rotation.toRotationMatrix();

    return model;
}
