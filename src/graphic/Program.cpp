#include "./Program.hpp"

#include <fstream>
#include <sstream>

#include "spdlog/spdlog.h"

enum class ShaderType : uint32_t { VERTEX, FRAGMENT };

char *getShaderErrorLog(GLuint shader) {
  GLint length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
  char *log = (char *)malloc(sizeof(char) * length);
  glGetShaderInfoLog(shader, length, nullptr, log);
  return log;
}

char *getProgramErrorLog(GLuint program) {
  GLint length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
  char *log = (char *)malloc(sizeof(char) * length);
  glGetProgramInfoLog(program, length, nullptr, log);
  return log;
}

GLuint compileShader(ShaderType type, const std::string &code) {
  GLenum shaderType =
      type == ShaderType::VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
  const char *codePtr = code.c_str();

  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &codePtr, nullptr);
  glCompileShader(shader);

  GLint status = 0;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    spdlog::error("Failed to compile shader {}", code);
    spdlog::error("{}", getShaderErrorLog(shader));
    glDeleteShader(shader);
    shader = 0;
  }

  return shader;
}

Program::Program(const std::string &vShaderCode,
                 const std::string &fShaderCode) {

  auto vShader = compileShader(ShaderType::VERTEX, vShaderCode);
  auto fShader = compileShader(ShaderType::FRAGMENT, fShaderCode);

  _id = glCreateProgram();
  assert(_id > 0 && "Program id illegal");

  glAttachShader(_id, vShader);
  glAttachShader(_id, fShader);
  glLinkProgram(_id);

  GLint status = 0;
  glGetProgramiv(_id, GL_LINK_STATUS, &status);
  if (GL_FALSE == status) {
    spdlog::error("Failed to link program");
    spdlog::error("{}", getShaderErrorLog(_id));
    glDeleteProgram(_id);
    _id = 0;
  }

  glDetachShader(_id, vShader);
  glDetachShader(_id, fShader);

  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

Program::~Program() {}

void Program::use() { glUseProgram(_id); }