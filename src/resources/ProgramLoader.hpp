#pragma once

#include <entt/entt.hpp>

#include "../graphic/Program.hpp"

struct ProgramLoader final : entt::resource_loader<ProgramLoader, Program> {
  std::shared_ptr<Program> load(
      const std::string &vShaderFilename,
      const std::string &fShaderFilename) const;
};