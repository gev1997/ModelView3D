// project
#include "viewer.h"

// third party

// std

gui::viewer::viewer(GLFWwindow* window)
{
    assert(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, _mouse_callback);
    glfwSetScrollCallback(window, _scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

gui::camera& gui::viewer::get_camera()
{
    return m_camera;
}

void gui::viewer::_mouse_callback(GLFWwindow* window, const double pos_x, const double pos_y)
{
    assert(window); // maybe if (!window) ?

    viewer* self = reinterpret_cast<viewer*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    
    self->_handle_mouse(pos_x, pos_y);
}

void gui::viewer::_scroll_callback(GLFWwindow* window, const double, const double offset_y)
{
    assert(window); // maybe if (!window) ?

    viewer* self = static_cast<viewer*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    
    self->_handle_scroll(offset_y);
}

void gui::viewer::_handle_mouse(const double pos_x, const double pos_y)
{
    if (m_first_mouse)
    {
        m_last_x = pos_x;
        m_last_y = pos_y;
        m_first_mouse = false;
    }

    const auto delta_x = static_cast<float>(pos_x - m_last_x);
    const auto delta_y = static_cast<float>(m_last_y - pos_y);

    m_last_x = pos_x;
    m_last_y = pos_y;

    m_camera.process_mouse_movement(delta_x, delta_y);
}

void gui::viewer::_handle_scroll(const double offset_y)
{
    m_camera.process_mouse_scroll(static_cast<float>(offset_y));
}
