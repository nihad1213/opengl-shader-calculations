#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.hpp"
#include "Shader.hpp"
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

    glEnable(GL_DEPTH_TEST);
    GLuint program = createProgram("shaders/phong.vert", "shaders/phong.frag");
    Mesh sphere = makeSphere(1.0f);

    while (!glfwWindowShouldClose(window)) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
            
        glClearColor(0.22f, 0.24f, 0.28f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        glm::mat4 model(1.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 5), glm::vec3(0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 50.0f);
            
        glUseProgram(program);
        setMat4(program, "model", model);
        setMat4(program, "view", view);
        setMat4(program, "projection", proj);
        sphere.draw();
            
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}