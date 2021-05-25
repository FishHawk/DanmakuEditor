#pragma once

#include "components/Launchable.hpp"
#include "resources/ResourceManager.hpp"

class TestCase {
public:
  TestCase(const ResourceManager &resource_manager);

  std::vector<Launchable> cases{};
  std::vector<char *> case_name{};

private:
  void add_test_case(char *name, Launchable launchable);

  const ResourceManager &_resource_manager;
};
