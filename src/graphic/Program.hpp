#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

class Program {
public:
  Program(const std::string &vShaderCode, const std::string &fShaderCode);

  ~Program();

  void use();

  inline void set_bool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
  }
  inline void set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
  }
  inline void set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
  }

  inline void set_vec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
  }
  inline void set_vec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(_id, name.c_str()), x, y);
  }
  inline void set_vec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
  }
  inline void set_vec3(const std::string &name, float x, float y,
                      float z) const {
    glUniform3f(glGetUniformLocation(_id, name.c_str()), x, y, z);
  }
  inline void set_vec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(_id, name.c_str()), 1, &value[0]);
  }
  inline void set_vec4(const std::string &name, float x, float y, float z,
                      float w) {
    glUniform4f(glGetUniformLocation(_id, name.c_str()), x, y, z, w);
  }

  inline void set_mat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  inline void set_mat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }
  inline void set_mat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
  }

private:
  GLuint _id = 0;
};
