#pragma once

// project
#include "namespace_gui.h"
#include "shader.h"

// third party
#include <Eigen/Geometry>

// std

class gui::camera
{
public:
    camera() = default;

    void process_mouse_move(float delta_x, float delta_y);
    void process_mouse_scroll(float offset_y);
    Eigen::Vector3f get_position() const;
    Eigen::Matrix4f get_view_matrix();
    Eigen::Matrix4f get_projection_matrix();
    Eigen::Matrix4f get_model_matrix();

private:
    // Camera state (orbit around target)
    float m_radius = 5.0f;
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;

    // Model rotation (optional spinning)
    float m_model_angle = 0.0f;
    bool m_auto_spin = false;

    // predefined values
    static constexpr float sensitivity = 0.005f;
    static constexpr float max_pitch = 1.57f;  // ~89 degrees
    static constexpr float field_of_view = 45.0f;
    static constexpr float aspect = 800.0f / 600.0f;
    static constexpr float z_near = 0.1f;
    static constexpr float z_far = 100.0f;
};
