#include "RepetitionGenerator.hpp"

namespace repetition {

Launchable::RepetitionGenerator at(Time time_point, size_t n) {
  return [=](auto, auto t) { return t == time_point ? n : 0; };
}

Launchable::RepetitionGenerator at_start(size_t n) {
  return [=](auto, auto t) { return t == 1 ? n : 0; };
}

Launchable::RepetitionGenerator at_end(size_t n) {
  return [=](auto duration, auto t) { return t == duration - 1 ? n : 0; };
}

Launchable::RepetitionGenerator every_n(Time period, size_t n) {
  return [=](auto, auto t) { return t % period == 0 ? 4 : 0; };
}

} // namespace repetition