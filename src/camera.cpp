// camera.cpp
#include "ModelView3D/camera.h"
#include "ModelView3D/shader.h"  // your shader wrapper
#include <iostream>

camera::camera()
{
    // Good default orientation
    yaw   = 0.0f;
    pitch = 0.2f;
}

void camera::process_mouse_movement(float delta_x, float delta_y)
{
    const float sensitivity = 0.005f;
    yaw   += delta_x * sensitivity;
    pitch += delta_y * sensitivity;

    const float max_pitch = 1.57f;  // ~89 degrees
    if (pitch > max_pitch)  pitch = max_pitch;
    if (pitch < -max_pitch) pitch = -max_pitch;
}

void camera::process_mouse_scroll(float offset_y)
{
    radius -= offset_y * 0.5f;
    if (radius < 0.1f) radius = 0.1f;
}

Eigen::Vector3f camera::_spherical_to_cartesian() const
{
    float x = radius * std::cos(pitch) * std::sin(yaw);
    float y = radius * std::sin(pitch);
    float z = radius * std::cos(pitch) * std::cos(yaw);
    return {x, y, z};
}

Eigen::Matrix4f camera::get_view_matrix()
{
    Eigen::Vector3f eye    = target + _spherical_to_cartesian();
    Eigen::Vector3f center = target;
    Eigen::Vector3f up     = Eigen::Vector3f(0,1,0);

    Eigen::Vector3f f = (center - eye).normalized();
    Eigen::Vector3f s = f.cross(up).normalized();
    Eigen::Vector3f u = s.cross(f);

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    view.block<1,3>(0,0) = s.transpose();
    view.block<1,3>(1,0) = u.transpose();
    view.block<1,3>(2,0) = -f.transpose();
    view(0,3) = -s.dot(eye);
    view(1,3) = -u.dot(eye);
    view(2,3) =  f.dot(eye);

    return view;
}

Eigen::Matrix4f camera::get_projection_matrix()
{
    float tanHalfFov = std::tan(fov * 0.008726646f);  // deg → rad / 2
    Eigen::Matrix4f proj = Eigen::Matrix4f::Zero();

    proj(0,0) = 1.0f / (aspect * tanHalfFov);
    proj(1,1) = 1.0f / tanHalfFov;
    proj(2,2) = -(z_far + z_near) / (z_far - z_near);
    proj(2,3) = -(2.0f * z_far * z_near) / (z_far - z_near);
    proj(3,2) = -1.0f;

    return proj;
}

Eigen::Matrix4f camera::get_model_matrix()
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    if (auto_spin) {
        model_angle += 0.3f;
        Eigen::AngleAxisf rot(model_angle * 0.017453292f, Eigen::Vector3f::UnitY());
        model.block<3,3>(0,0) = rot.toRotationMatrix();
    }
    return model;
}

void camera::update_uniforms(shader_program* shader, const std::string& prefix)
{
    shader->set_matrix4((prefix + "model").c_str(),      get_model_matrix().data());
    shader->set_matrix4((prefix + "view").c_str(),       get_view_matrix().data());
    shader->set_matrix4((prefix + "projection").c_str(), get_projection_matrix().data());
}
