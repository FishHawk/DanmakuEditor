#include "TestCase.hpp"

#include <random>

#include "components/Livetime.hpp"
#include "components/Sprite.hpp"
#include "generators/DirectionGenerator.hpp"
#include "generators/ModifierGenerator.hpp"
#include "generators/OriginGenerator.hpp"
#include "generators/RepetitionGenerator.hpp"

std::default_random_engine generator;
std::uniform_real_distribution<Distance> distribution(-1, 1);
auto unifom = std::bind(distribution, generator);

const auto case1 = Launchable{
    0,
    -1,
    repetition::at_start(),
    origin::at(Position{300, 400}),
    nullptr,
    nullptr,
    []() {
      return Launchable{
          1,
          -1,
          repetition::at_start(4),
          origin::follow(),
          direction::circle(0, glm::radians(360.f), 4),
          modifier::by_position(modifier::polar_rotated(
              [](Direction d, Time t) { return 32 + 16 * sin(0.1 * t); },
              [](Direction d, Time t) { return 0.01 * t; })),
          []() {
            return Launchable{
                2,
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
    0,
    -1,
    repetition::at_start(),
    origin::at(Position{300, 400}),
    nullptr,
    nullptr,
    []() {
      return Launchable{
          1,
          12,
          repetition::every_n(12),
          origin::at([](auto p, auto t, auto i) {
            return p + Position{200 * unifom(), -300 + 100 * unifom()};
          }),
          nullptr,
          nullptr,
          []() {
            auto a = unifom();
            return Launchable{
                2,
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

std::vector<char *> TestCase::case_name{"default", "case1", "case2"};
std::vector<Launchable> TestCase::cases{case2, case1, case2};