#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

inline GLuint compileShader(GLenum type, const std::string &path) {
    std::ifstream f(path);
    if (!f) std::cerr << "Cannot open " << path << "\n";
    std::stringstream ss;
    ss << f.rdbuf();
    std::string src = ss.str();
    const char *c = src.c_str();

    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);

    GLint ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(s, 1024, nullptr, log);
        std::cerr << "Compile error in " << path << ":\n" << log << "\n";
    }
    return s;
}

inline GLuint createProgram(const std::string &vertPath, const std::string &fragPath) {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertPath);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragPath);
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

inline GLint U(GLuint p, const char *n) { return glGetUniformLocation(p, n); }
inline void setInt(GLuint p, const char *n, int v)               { glUniform1i(U(p, n), v); }
inline void setVec3(GLuint p, const char *n, const glm::vec3 &v) { glUniform3fv(U(p, n), 1, glm::value_ptr(v)); }
inline void setMat3(GLuint p, const char *n, const glm::mat3 &m) { glUniformMatrix3fv(U(p, n), 1, GL_FALSE, glm::value_ptr(m)); }
inline void setMat4(GLuint p, const char *n, const glm::mat4 &m) { glUniformMatrix4fv(U(p, n), 1, GL_FALSE, glm::value_ptr(m)); }