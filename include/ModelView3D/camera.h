// camera.h
#pragma once

#include <Eigen/Geometry>

class camera
{
public:
    camera();

    // Input
    void process_mouse_movement(float delta_x, float delta_y);
    void process_mouse_scroll(float offset_y);

    // Call every frame before drawing
    void update_uniforms(class shader_program* shader, const std::string& prefix = "");

    // Getters (rarely needed now)
    Eigen::Matrix4f get_view_matrix();
    Eigen::Matrix4f get_projection_matrix();
    Eigen::Matrix4f get_model_matrix();

private:
    Eigen::Vector3f _spherical_to_cartesian() const;

private:
    // Camera state (orbit around target)
    Eigen::Vector3f target = Eigen::Vector3f::Zero();
    float radius = 5.0f;
    float yaw    = 0.0f;
    float pitch  = 0.0f;

    // Projection settings
    float fov    = 45.0f;
    float aspect = 800.0f / 600.0f;
    float z_near = 0.1f;
    float z_far  = 100.0f;

    // Model rotation (optional spinning)
    float model_angle = 0.0f;
    bool  auto_spin   = true;
};
