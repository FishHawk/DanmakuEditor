#include "ProgramLoader.hpp"

#include <fstream>
#include <sstream>

#include <spdlog/spdlog.h>

std::string read_shader_file(const std::string &filename) {
  std::ifstream file;
  std::stringstream file_stream;

  try {
    file.open(filename);
    file_stream << file.rdbuf();
    file.close();
  } catch (std::exception e) {
    spdlog::error("ERROR::SHADER: Failed to read shader files");
  }

  return file_stream.str();
}

std::shared_ptr<Program> ProgramLoader::load(
    const std::string &vShaderFilename,
    const std::string &fShaderFilename) const {
  auto ptr = new Program{
      read_shader_file(vShaderFilename), read_shader_file(fShaderFilename)};
  return std::shared_ptr<Program>{ptr};
}