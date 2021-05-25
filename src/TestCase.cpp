#include "TestCase.hpp"

#include <random>

#include "components/Livetime.hpp"
#include "components/Sprite.hpp"
#include "generators/DirectionGenerator.hpp"
#include "generators/ModifierGenerator.hpp"
#include "generators/OriginGenerator.hpp"
#include "generators/RepetitionGenerator.hpp"

using namespace entt::literals;

std::default_random_engine generator;
std::uniform_real_distribution<Distance> distribution(-1, 1);
auto unifom = std::bind(distribution, generator);

TestCase::TestCase(const ResourceManager &resource_manager)
    : _resource_manager(resource_manager) {
  auto aircraft = _resource_manager.sprite_frame_cache().handle("aircraft"_hs);
  auto cannon = _resource_manager.sprite_frame_cache().handle("cannon"_hs);
  auto bullet = _resource_manager.sprite_frame_cache().handle("bullet"_hs);

  const auto case1 = Launchable{
      aircraft,
      -1,
      repetition::at_start(),
      origin::at(Position{300, 400}),
      nullptr,
      nullptr,
      [=]() {
        return Launchable{
            cannon,
            -1,
            repetition::at_start(4),
            origin::follow(),
            direction::circle(0, glm::radians(360.f), 4),
            modifier::by_position(modifier::polar_rotated(
                [](Direction d, Time t) { return 32 + 16 * sin(0.1 * t); },
                [](Direction d, Time t) { return 0.01 * t; })),
            [=]() {
              return Launchable{
                  bullet,
                  120,
                  repetition::every_n(1),
                  origin::at(),
                  direction::at(),
                  modifier::by_velocity(modifier::cartesian_rotated(
                      [](Direction d, Time t) { return 1; },
                      [](Direction d, Time t) { return 0; }))};
            }};
      }};

  const auto case2 = Launchable{
      aircraft,
      -1,
      repetition::at_start(),
      origin::at(Position{300, 400}),
      nullptr,
      nullptr,
      [=]() {
        return Launchable{
            cannon,
            12,
            repetition::every_n(12),
            origin::at([](auto p, auto t, auto i) {
              return p + Position{200 * unifom(), -300 + 100 * unifom()};
            }),
            nullptr,
            nullptr,
            [=]() {
              auto a = unifom();
              return Launchable{
                  bullet,
                  600,
                  repetition::every_n(1),
                  origin::at(),
                  direction::at([=](auto d, auto t, auto i) {
                    return glm::radians(10.f) * a;
                  }),
                  modifier::by_velocity(modifier::cartesian_rotated(
                      [](Direction d, Time t) { return 0; },
                      [](Direction d, Time t) { return 1; }))};
            }};
      }};

  auto default_case = case1;
  add_test_case("default", default_case);
  add_test_case("case1", case1);
  add_test_case("case2", case2);
}

void TestCase::add_test_case(char *name, Launchable launchable) {
  case_name.push_back(name);
  cases.push_back(launchable);
}