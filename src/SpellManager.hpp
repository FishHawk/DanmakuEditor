#pragma once

#include "components/Launchable.hpp"

class SpellManager {
public:
  SpellManager();

  std::vector<Launchable> spells{};
  std::vector<char *> spell_names{};

private:
  void add_spell(char *name, Launchable spell);
};
