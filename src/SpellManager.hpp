#pragma once

#include "components/Launchable.hpp"
#include "resources/ResourceManager.hpp"

class SpellManager {
public:
  SpellManager(const ResourceManager &resource_manager);

  std::vector<Launchable> spells{};
  std::vector<char *> spell_names{};

private:
  void add_spell(char *name, Launchable spell);

  const ResourceManager &_resource_manager;
};
