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
    static void _mouse_move_callback(GLFWwindow* window, double pos_x, double pos_y);
    static void _mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void _mouse_scroll_callback(GLFWwindow* window, double offset_x, double offset_y);
    void _handle_mouse_move(double pos_x, double pos_y);
    void _handle_mouse_button(int button, int action);
    void _handle_mouse_scroll(double offset_y);

private:
    camera m_camera;
    bool m_rotating = false;
    bool m_first_mouse = true;
    double m_last_x = 0;
    double m_last_y = 0;
};