#ifndef GUI_H
#define GUI_H
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void imgui_init(GLFWwindow *window);
void imgui_create();
void imgui_end();

#endif