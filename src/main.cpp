#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *Window = glfwCreateWindow(1200, 760, "Window", NULL, NULL);

    if (!Window) {
        std::cout << "Window has failed to be created, terminating.\n";
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(Window);

    gladLoadGL();

    glViewport(0, 0, 800, 800);

    glClearColor(0.0f, 0.5f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(Window);
    
    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
    }
    
    glfwDestroyWindow(Window);
    glfwTerminate();
    
    return 0;

    return 0;
}