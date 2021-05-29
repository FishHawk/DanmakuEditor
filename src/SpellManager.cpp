#include "SpellManager.hpp"

#include <random>

#include "components/Livetime.hpp"
#include "components/Renderable.hpp"
#include "generators/DirectionGenerator.hpp"
#include "generators/ModifierGenerator.hpp"
#include "generators/OriginGenerator.hpp"
#include "generators/RepetitionGenerator.hpp"

using namespace entt::literals;

std::default_random_engine generator;
std::uniform_real_distribution<Distance> distribution(-1, 1);
auto unifom = std::bind(distribution, generator);

SpellManager::SpellManager(const ResourceManager &resource_manager)
    : _resource_manager(resource_manager) {
  const auto spell1 = Launchable{
      "aircraft"_hs,
      -1,
      repetition::at_start(),
      origin::at(Position{300, 400}),
      nullptr,
      nullptr,
      [=]() {
        return Launchable{
            "cannon"_hs,
            -1,
            repetition::at_start(128),
            origin::follow(),
            direction::circle(0, glm::radians(360.f), 128),
            modifier::by_position(modifier::polar_rotated(
                [](Direction d, Time t) { return 32 + 16 * sin(0.1 * t); },
                [](Direction d, Time t) { return 0.01 * t; })),
            [=]() {
              return Launchable{
                  "bullet"_hs,
                  120,
                  repetition::every_n(1),
                  origin::at(),
                  direction::at(),
                  modifier::by_velocity(modifier::cartesian_rotated(
                      [](Direction d, Time t) { return 1; },
                      [](Direction d, Time t) { return 0; }))};
            }};
      }};

  const auto spell2 = Launchable{
      "aircraft"_hs,
      -1,
      repetition::at_start(),
      origin::at(Position{300, 400}),
      nullptr,
      nullptr,
      [=]() {
        return Launchable{
            "cannon"_hs,
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
                  "bullet"_hs,
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

  auto default_spell = spell1;
  add_spell("default", default_spell);
  add_spell("spell1", spell1);
  add_spell("spell2", spell2);
}

void SpellManager::add_spell(char *name, Launchable spell) {
  spell_names.push_back(name);
  spells.push_back(spell);
}