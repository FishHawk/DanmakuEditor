#pragma once

#include "components/Launchable.hpp"

class TestCase {
public:
  static std::vector<Launchable> cases;
  static std::vector<char *> case_name;
};
