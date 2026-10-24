#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include "Mesh.hpp"
#include "Shader.hpp"

int currentShape = 0;
int currentMode = 0;
bool lightOrbits = true;
bool dragging = false;
double lastX = 0, lastY = 0;
float rotX = 20.0f, rotY = -30.0f;

const char *shapeNames[] = {"Sphere", "Cube", "Cylinder"};
const char *modeNames[]  = {"Full Phong", "Ambient", "Diffuse", "Specular", "Normals"};

void updateTitle(GLFWwindow *w) {
    std::string t = std::string("Shading demo | ") + shapeNames[currentShape] + " | " + modeNames[currentMode];
    glfwSetWindowTitle(w, t.c_str());
}

void onResize(GLFWwindow *, int w, int h) { glViewport(0, 0, w, h); }

void onKey(GLFWwindow *w, int key, int, int action, int) {
    if (action != GLFW_PRESS) return;
    if (key == GLFW_KEY_1) currentShape = 0;
    if (key == GLFW_KEY_2) currentShape = 1;
    if (key == GLFW_KEY_3) currentShape = 2;
    if (key == GLFW_KEY_M) currentMode = (currentMode + 1) % 5;
    if (key == GLFW_KEY_L) lightOrbits = !lightOrbits;
    if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(w, GLFW_TRUE);
    updateTitle(w);
}

void onMouseButton(GLFWwindow *w, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        dragging = (action == GLFW_PRESS);
        glfwGetCursorPos(w, &lastX, &lastY);
    }
}

void onCursor(GLFWwindow *, double x, double y) {
    if (!dragging) return;
    rotY += (float)(x - lastX) * 0.4f;
    rotX += (float)(y - lastY) * 0.4f;
    lastX = x;
    lastY = y;
}

int main() {
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow *window = glfwCreateWindow(1200, 760, "Shading demo", nullptr, nullptr);
    if (!window) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGL();

    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetKeyCallback(window, onKey);
    glfwSetMouseButtonCallback(window, onMouseButton);
    glfwSetCursorPosCallback(window, onCursor);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    GLuint program = createProgram("shaders/phong.vert", "shaders/phong.frag");
    std::vector<Mesh> shapes = {makeSphere(), makeCube(), makeCylinder()};
    Mesh lightMarker = makeSphere(0.08f, 16, 24);

    const glm::vec3 viewPos(0, 0, 5);
    float lightAngle = 0.8f;
    double prev = glfwGetTime();
    updateTitle(window);

    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        float dt = (float)(now - prev);
        prev = now;
        if (lightOrbits) lightAngle += dt * 0.8f;

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);

        glClearColor(0.22f, 0.24f, 0.28f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightPos(3.2f * std::cos(lightAngle), 1.2f, 3.2f * std::sin(lightAngle));

        glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)w / h, 0.1f, 50.0f);

        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(rotX), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotY), glm::vec3(0, 1, 0));
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

        glUseProgram(program);
        setMat4(program, "view", view);
        setMat4(program, "projection", proj);
        setVec3(program, "lightPos", lightPos);
        setVec3(program, "lightColor", glm::vec3(1.0f));
        setVec3(program, "viewPos", viewPos);
        setVec3(program, "objectColor", glm::vec3(0.95f, 0.55f, 0.20f));
        setInt(program, "mode", currentMode);

        setInt(program, "unlit", 0);
        setMat4(program, "model", model);
        setMat3(program, "normalMatrix", normalMatrix);
        shapes[currentShape].draw();

        setInt(program, "unlit", 1);
        setMat4(program, "model", glm::translate(glm::mat4(1.0f), lightPos));
        setMat3(program, "normalMatrix", glm::mat3(1.0f));
        lightMarker.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}