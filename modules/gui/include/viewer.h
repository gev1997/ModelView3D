#pragma once

// project
#include "namespace_gui.h"
#include "camera.h"

// third party
#include <GLFW/glfw3.h>

// std

class gui::viewer // class eye?
{
public:
    viewer(GLFWwindow* window);
    camera& get_camera();
    
private:
    static void _mouse_callback(GLFWwindow* window, const double pos_x, const double pos_y);
    static void _scroll_callback(GLFWwindow* window, const double offset_x, const double offset_y);
    void _handle_mouse(const double pos_x, const double pos_y);
    void _handle_scroll(const double offset_y);

private:
    camera m_camera;
    bool m_first_mouse = true;
    double m_last_x = 0;
    double m_last_y = 0;
};