// project
#include "viewer.h"

// third party

// std

gui::viewer::viewer(GLFWwindow* window)
{
    assert(window);

    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, _mouse_move_callback);
    glfwSetMouseButtonCallback(window, _mouse_button_callback);
    glfwSetScrollCallback(window, _mouse_scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

gui::camera& gui::viewer::get_camera()
{
    return m_camera;
}

void gui::viewer::_mouse_move_callback(GLFWwindow* window, double pos_x, double pos_y)
{
    assert(window); // maybe if (!window) ?

    viewer* self = reinterpret_cast<viewer*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    
    self->_handle_mouse_move(pos_x, pos_y);
}

void gui::viewer::_mouse_button_callback(GLFWwindow* window, int button, int action, int)
{
    assert(window); // maybe if (!window) ?

    viewer* self = reinterpret_cast<viewer*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    
    self->_handle_mouse_button(button, action);
}

void gui::viewer::_mouse_scroll_callback(GLFWwindow* window, double, double offset_y)
{
    assert(window); // maybe if (!window) ?

    viewer* self = static_cast<viewer*>(glfwGetWindowUserPointer(window));
    if (!self)
        return;
    
    self->_handle_mouse_scroll(offset_y);
}

void gui::viewer::_handle_mouse_move(double pos_x, double pos_y)
{
    if (!m_rotating)
        return;
    
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

    m_camera.process_mouse_move(delta_x, delta_y);
}

void gui::viewer::_handle_mouse_button(int button, int action)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    switch (action)
    {
        case GLFW_PRESS:
            m_rotating = true;
            m_first_mouse = true;
            break;
        case GLFW_RELEASE:
            m_rotating = false;
            break;
        default:
            break;
    }
}

void gui::viewer::_handle_mouse_scroll(double offset_y)
{
    m_camera.process_mouse_scroll(static_cast<float>(offset_y));
}
