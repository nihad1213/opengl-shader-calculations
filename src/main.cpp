#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

void onResize(GLFWwindow *, int w, int h) { glViewport(0, 0, w, h); }

int main() {

    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1200, 760, "Shading Demo", nullptr, nullptr);
    if (!window) {
        std::cout << "Window has failed to be created, terminating.\n";
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetFramebufferSizeCallback(window, onResize);


    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.22f, 0.24f, 0.28f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}