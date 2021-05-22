#pragma once

#include <map>
#include <string>

#include "Program.hpp"
#include "Texture.hpp"

class ResourceManager {
public:
  static Program add_program(const std::string &name,
                             const std::string &vShaderFilename,
                             const std::string &fShaderFilename);

  static Program get_program(const std::string &name);

  static Texture add_texture(const std::string &name, const std::string &file,
                             bool alpha);
  static Texture get_texture(const std::string &name);

  static void clear();

private:
  ResourceManager() {}

  static std::map<std::string, Program> programs;
  static std::map<std::string, Texture> textures;

  static Program load_program_from_file(const std::string &vShaderFilename,
                                        const std::string &fShaderFilename);

  static Texture load_texture_from_file(const char *file, bool alpha);
};
